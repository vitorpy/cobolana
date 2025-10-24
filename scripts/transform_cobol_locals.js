#!/usr/bin/env node

// Transform the cobc-generated local header into a stack-based state overlay.
// The script replaces static storage with macros that reference a COBOLState
// instance, and emits runtime assignments for cob_field metadata.

const fs = require('fs');
const path = require('path');

if (process.argv.length < 3) {
  console.error('Usage: transform_cobol_locals.js <path-to-cobolana.c.l.h>');
  process.exit(1);
}

const headerPath = process.argv[2];
const source = fs.readFileSync(headerPath, 'utf8');
const lines = source.split(/\r?\n/);

const commentLines = [];
let firstNonCommentIndex = 0;
for (; firstNonCommentIndex < lines.length; firstNonCommentIndex++) {
  const trimmed = lines[firstNonCommentIndex].trim();
  if (trimmed === '' || trimmed.startsWith('/*')) {
    commentLines.push(lines[firstNonCommentIndex]);
  } else {
    break;
  }
}

const identifierSet = new Set();
const fieldInits = [];

const attributePattern = /__attribute__\s*\(\([^)]*\)\)/g;

function registerIdentifierFromLine(rawLine) {
  let line = rawLine;
  const commentStart = line.indexOf('/*');
  if (commentStart !== -1) {
    line = line.slice(0, commentStart);
  }
  line = line.trim();
  if (!line.endsWith(';')) {
    return;
  }

  const fieldInitMatch = line.match(/static\s+cob_field\s+([A-Za-z_]\w*)\s*=\s*\{([^}]*)\}/);
  if (fieldInitMatch) {
    fieldInits.push({
      name: fieldInitMatch[1],
      values: fieldInitMatch[2].split(',').map((v) => v.trim()).filter(Boolean),
    });
  }

  let declaration = line.slice(0, -1); // drop trailing ';'
  declaration = declaration.replace(/^static\s+/, '');
  declaration = declaration.replace(attributePattern, '');
  const eqIndex = declaration.indexOf('=');
  if (eqIndex !== -1) {
    declaration = declaration.slice(0, eqIndex);
  }
  declaration = declaration.trim();
  if (!declaration) {
    return;
  }

  const nameMatch = declaration.match(/([A-Za-z_]\w*)(?:\s*(?:\[[^\]]*\]))?$/);
  if (nameMatch) {
    identifierSet.add(nameMatch[1]);
  }
}

for (let i = firstNonCommentIndex; i < lines.length; i++) {
  const trimmed = lines[i].trim();
  if (!trimmed || trimmed.startsWith('/*')) {
    continue;
  }
  if (trimmed.startsWith('#')) {
    continue;
  }
  registerIdentifierFromLine(lines[i]);
}

function transformValue(value) {
  if (identifierSet.has(value)) {
    return `cobol_state.${value}`;
  }
  // Handle pointer references like &foo
  if (value.startsWith('&')) {
    const bare = value.slice(1);
    if (identifierSet.has(bare)) {
      return `&cobol_state.${bare}`;
    }
  }
  return value;
}

const fieldInitAssignments = fieldInits.flatMap(({ name, values }) => {
  const transformed = values.map(transformValue);
  const assignments = [];
  if (transformed[0]) {
    assignments.push(`cobol_state.${name}.size = ${transformed[0]};`);
  }
  if (transformed[1]) {
    assignments.push(`cobol_state.${name}.data = ${transformed[1]};`);
  }
  if (transformed[2]) {
    assignments.push(`cobol_state.${name}.attr = ${transformed[2]};`);
  }
  return assignments;
});

const overlayLines = [];
overlayLines.push('#ifndef COBOLANA_LOCALS_OVERLAY_H');
overlayLines.push('#define COBOLANA_LOCALS_OVERLAY_H');
overlayLines.push('');
overlayLines.push('COBOLState cobol_state;');
overlayLines.push('cobol_state_init(&cobol_state);');
fieldInitAssignments.forEach((stmt) => {
  overlayLines.push(stmt);
});
if (identifierSet.has('cob_procedure_params')) {
  overlayLines.push('cob_field **cob_procedure_params = cobol_state.cob_procedure_params;');
  overlayLines.push('');
  identifierSet.delete('cob_procedure_params');
}
overlayLines.push('');
const identifiers = Array.from(identifierSet).sort();
identifiers.forEach((name) => {
  overlayLines.push(`#pragma push_macro("${name}")`);
  overlayLines.push(`#undef ${name}`);
  overlayLines.push(`#define ${name} (cobol_state.${name})`);
  overlayLines.push('');
});
overlayLines.push(`#pragma push_macro("cob_module_global_enter")`);
overlayLines.push('#undef cob_module_global_enter');
overlayLines.push('#define cob_module_global_enter(module_ptr, glob_ptr, auto_init, entry, check) \\');
overlayLines.push('  cob_module_global_enter_state(&cobol_state, module_ptr, glob_ptr, auto_init, entry, check)');
overlayLines.push('');
overlayLines.push('#endif /* COBOLANA_LOCALS_OVERLAY_H */');
overlayLines.push('');

const cleanupLines = [];
cleanupLines.push('#ifndef COBOLANA_LOCALS_OVERLAY_CLEANUP_H');
cleanupLines.push('#define COBOLANA_LOCALS_OVERLAY_CLEANUP_H');
cleanupLines.push('');
cleanupLines.push('#pragma pop_macro("cob_module_global_enter")');
cleanupLines.push('');
for (let i = identifiers.length - 1; i >= 0; i--) {
  const name = identifiers[i];
  cleanupLines.push(`#pragma pop_macro("${name}")`);
}
cleanupLines.push('');
cleanupLines.push('#endif /* COBOLANA_LOCALS_OVERLAY_CLEANUP_H */');
cleanupLines.push('');

const dir = path.dirname(headerPath);
const overlayPath = path.join(dir, 'cobolana_locals_overlay.h');
const cleanupPath = path.join(dir, 'cobolana_locals_cleanup.h');

fs.writeFileSync(overlayPath, overlayLines.join('\n'));
fs.writeFileSync(cleanupPath, cleanupLines.join('\n'));

const wrapperLines = [...commentLines, '#include "cobolana_locals_overlay.h"', ''];
fs.writeFileSync(headerPath, wrapperLines.join('\n'));

const patchedPath = path.join(dir, 'cobolana_patched.c');
if (fs.existsSync(patchedPath)) {
  const includeLine = '#include "cobolana_locals_cleanup.h"';
  let patchedSource = fs.readFileSync(patchedPath, 'utf8');
  if (!patchedSource.includes(includeLine)) {
    const marker = 'static void COBOLANA_module_init';
    const idx = patchedSource.indexOf(marker);
    if (idx === -1) {
      console.error('Failed to locate COBOLANA_module_init in cobolana_patched.c');
      process.exit(1);
    }
    const insertion = `${includeLine}\n\n`;
    patchedSource = patchedSource.slice(0, idx) + insertion + patchedSource.slice(idx);
    fs.writeFileSync(patchedPath, patchedSource);
  }
}
