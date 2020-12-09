import express from 'express';
import moment from 'moment';
import firebase from './firebase';

const app = express();
const router = express.Router();

const getDatetime = () => moment().format('YYYY-MM-DD HH:mm:ss');

const post = async (req, res) => {
  const { type, temperatura, umidade, datetime } = req.body;

  let valuesRef = firebase.database().ref('dados');

  return valuesRef.push({
    type,
    temperatura,
    umidade,
    datetime: getDatetime(),
  }, (error) => {
    if (error) {
      return res.send(500).send({ message: 'deu merda' })
    } else {
      return res.send(200);
    }
  });
};

const get = (req, res) => {
  const valuesRef = firebase.database().ref('dados');

  return valuesRef.on('value', (snapshot) => {
    const value = snapshot.val();

    const values = Object.keys(value).map((key) => ({ key, ...value[key] }));

    return res.status(200).send({ values });
  }, (error) => res.status(500).send({ message: error.code }));
};


router.get('/', get);
router.post('/', post);

app.use('/', router);

export default app;
