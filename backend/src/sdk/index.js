import registerLib from '../lib';
import registerApi from '../api';

export const registerSdk = (app, apiPath = '/') => {
  app.use(apiPath, registerApi());
  app.use(apiPath, registerLib());

  return app;
};

export default registerSdk;
