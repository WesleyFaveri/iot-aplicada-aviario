const databaseErrorHandler = (error, req, res, next) => {
  if (error instanceof Error) {
    return res.status(503).json({
      type: 'PostgresError',
      message: error.message
    });
  }
  next(error);
};

export default databaseErrorHandler;
