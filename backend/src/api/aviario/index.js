import express from 'express';
import alive from './alive';

const app = express();

app.use(alive);

export default app;
