import http from 'http';
import express from 'express';
import cors from 'cors';
import bodyParser from 'body-parser';
import methodOverride from 'method-override';
import registerSdk from './sdk';
import logger from 'morgan';
import dotenv from 'dotenv';

dotenv.config();

const app = express();

const environment = process.env.NODE_ENV; // development

if (environment !== 'production') {
  app.use(logger('dev'));
}

const { PORT } = process.env;
app.use(cors());
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));
app.use(methodOverride());

registerSdk(app);

const httpServer = http.createServer(app);

httpServer.listen(PORT, () => {
  // eslint-disable-next-line no-console
  console.log(`🚀 Aviário srv running at http://localhost:${PORT}`);
});

export default app;
