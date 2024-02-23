import React from 'react';
import ReactDOM from 'react-dom/client';
import { Provider } from 'react-redux';
import { store } from './stores/store';
import Router from './routes';
import Theme from './theme';
import Background from './components/Background';

const root = ReactDOM.createRoot(
  document.getElementById('root') as HTMLElement
);

document.getElementById("simple-loader")?.remove();

setTimeout(() => {
  console.log("App started");

  root.render(
    <Provider store={store} key="app">
      <Theme>
        <Background />
        <Router />
      </Theme>
    </Provider>
  );
}, 1);
