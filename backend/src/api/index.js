import express from 'express';
import aviario from './aviario';

export default () => {
  const router = express.Router();

  router.use('/', aviario);

  return router;
};
