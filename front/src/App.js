import { useState } from 'react';
import './App.css';
import firebase from "firebase/app";
import "firebase/database";
import {
  FirebaseDatabaseProvider,
  FirebaseDatabaseNode
} from "@react-firebase/database";
import ReactLoading from 'react-loading';
import moment from 'moment';
import { ToastContainer, toast } from 'react-toastify';

import 'react-toastify/dist/ReactToastify.css';

const firebaseConfig = {
};

const constantsAviario = [
  { type: 1, state: 3 },
  { type: 2, state: 5 },
  { type: 3, state: 2 },
  { type: 4, state: 4 },
  { type: 5, state: 5 },
  { type: 6, state: 5 },
];

const constantsTemperatura = [
  { state: 1, maxTemp: 35, minTemp: 31, maxUmi: 70, minUmi: 60 },
  { state: 2, maxTemp: 32, minTemp: 29, maxUmi: 70, minUmi: 60 },
  { state: 3, maxTemp: 29, minTemp: 26, maxUmi: 70, minUmi: 60 },
  { state: 4, maxTemp: 26, minTemp: 23, maxUmi: 70, minUmi: 60 },
  { state: 5, maxTemp: 23, minTemp: 20, maxUmi: 70, minUmi: 60 },
];

const getSortedList = (list) => {
  const sortedList = list.sort((a, b) => {
    if (b.datetime < a.datetime) {
      return -1;
    }
    if (b.datetime > a.datetime) {
      return 1;
    }

    return 0;
  });

  return sortedList;
}

const getFormatedData = (values) => getSections(values, 'type');

const getDistinct = (array, field) => [...new Set(array.map(item => item[field]))];

const getSections = (array, field) => getDistinct(array, field).map((section) => {
  const values = getSortedList(array.filter((item) => item[field] === section));

  return {
    title: section,
    values,
    lastValue: values[0],
  }
});

const getSituacao = (current, lastValues, last, setLast) => {
  const currentState = constantsAviario.find((item) => item.type === parseInt(current.type));

  const neededValues = constantsTemperatura.find((item) => item.state === currentState.state);

  const currentTemp = parseInt(current.temperatura);
  const currentUmi = parseInt(current.umidade);

  const tempOk = !!((currentTemp >= neededValues.minTemp) && (currentTemp <= neededValues.maxTemp));
  const umiOk = !!((currentUmi >= neededValues.minUmi) && (currentUmi <= neededValues.maxUmi));

  let color = '#1DB013';
  let textTemp = null;
  let textUmi = null;

  if ((!tempOk) || (!umiOk)) {
    color = '#F31709';

    if (!tempOk) {
      textTemp = (currentTemp < neededValues.minTemp) ? 'Temperatura abaixo do ideal' : 'Temperatura acima do ideal';
      if (last.key !== current.key) {
        toast.warn(`Aviário - ${current.type} - ${textTemp}`, { toastId: current.key + 'temp' });
      }
    }

    if (!umiOk) {
      textUmi = (currentUmi < neededValues.minUmi) ? 'Umidade abaixo do ideal' : 'Umidade acima do ideal';
      if (last.key !== current.key) {
        toast.warn(`Aviário - ${current.type} - ${textUmi}`, { toastId: current.key + 'umi' });
      }
    }
  }

  setLast([...lastValues.filter((item) => item.type !== current.type), current]);

  return {
    color,
    textUmi,
    textTemp
  }
};

function App() {
  const [lastValues, setLastValues] = useState([]);

  return (
    <FirebaseDatabaseProvider firebase={firebase} {...firebaseConfig}>
      <div className="App">
        <header className="App-header">
          <FirebaseDatabaseNode
            path="dados/"
            orderByKey
          >
            {(data) => {
              if ((!data.isLoading) && (!!data.value)) {
                const { value } = data;

                const values = Object.keys(value).map((key) => ({ key, ...value[key] }));

                const dados = getFormatedData(values);

                return (
                  <div style={{ flex: 0, display: 'flex', flexDirection: 'row' }}>
                    {dados.map((item) => {
                      const last = lastValues.length > 0 ? lastValues.find((item) => lastValues.type === item.title) : { key: null, type: null };

                      const infoSituacao = getSituacao(item.lastValue, lastValues, last, setLastValues);

                      return (
                        <div key={item.title} style={{ borderWidth: 2, border: 'solid', borderRadius: 8, borderColor: 'grey', padding: 20, paddingTop: 50, paddingBottom: 50, marginLeft: 10, marginRight: 10 }}>
                          <span style={{ fontSize: 23, color: 'white', paddingTop: 50 }}>Aviário - {item.title}</span>

                          <div style={{ flex: 0, display: 'flex', flexDirection: 'row' }}>
                            <div style={{ flex: 0, display: 'flex', flexDirection: 'column', padding: 50 }}>
                              <span style={{ fontSize: 22, color: 'white' }}>Temperatura</span>
                              <span style={{ fontSize: 20, color: 'white', marginTop: 15 }}>{item.lastValue.temperatura}°C</span>
                            </div>

                            <div style={{ flex: 0, display: 'flex', flexDirection: 'column', padding: 50 }}>
                              <span style={{ fontSize: 22, color: 'white' }}>Umidade</span>
                              <span style={{ fontSize: 20, color: 'white', marginTop: 15 }}>{item.lastValue.umidade}%</span>
                            </div>
                          </div>

                          <div style={{ flex: 0, display: 'flex', flexDirection: 'column' }} >
                            <span style={{ fontSize: 22, color: 'white' }}>Situação</span>

                            <div style={{ marginTop: 20, height: 30, width: 200, alignSelf: 'center', backgroundColor: infoSituacao.color }} />

                            <div style={{ marginTop: 15, display: 'flex', flexDirection: 'column' }}>
                              {infoSituacao.textTemp && (<span style={{ fontSize: 18, marginTop: 5, color: 'white' }}>{infoSituacao.textTemp}</span>)}
                              {infoSituacao.textUmi && (<span style={{ fontSize: 18, marginTop: 5, color: 'white' }}>{infoSituacao.textUmi}</span>)}
                            </div>
                          </div>

                          <div style={{ flex: 0, display: 'flex', flexDirection: 'column', marginTop: 20 }} >
                            <span style={{ fontSize: 22, color: 'white' }}>Última atualização</span>

                            <span style={{ fontSize: 18, marginTop: 15, color: 'white' }}>{moment(item.lastValue.datetime).format('DD/MM/YYYY - HH:mm:ss')}</span>
                          </div>
                        </div>
                      )
                    })}
                  </div>
                );
              } else {
                return (
                  <ReactLoading type={'balls'} color={'white'} height={'5%'} width={'5%'} />
                );
              }
            }}
          </FirebaseDatabaseNode>
        </header>
        <ToastContainer
          position="top-right"
          autoClose={30000}
          hideProgressBar={false}
          newestOnTop={true}
          closeOnClick
          rtl={false}
          pauseOnFocusLoss
          draggable
          pause
        />
      </div>
    </FirebaseDatabaseProvider>
  );
}

export default App;
