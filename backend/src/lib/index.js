import express from 'express';
import { databaseErrorHandler, defaultErrorHandler } from './errors';

export default () => {
  const app = express();

  app.use(databaseErrorHandler);
  app.use(defaultErrorHandler);

  return app;
};
