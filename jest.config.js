module.exports = {
  preset: 'ts-jest',
  testEnvironment: 'node',
  testTimeout: 120000, // 120 seconds (test validator startup)
  testMatch: ['**/test/**/*.test.ts'],
};
