import Vue from 'vue'
import Vuex from 'vuex'

import Ball from '../ball.js'

Vue.use(Vuex);

const kMaxBalls = 8;

function initBalls() {
  let balls = new Array(kMaxBalls);
  for (let i = 0; i < kMaxBalls; i++) {
    balls[i] = new Ball(0, 0, 0, 0, 1);
  }
  return balls;
}

function clamp(val, min, max) {
  if (val < min) val = min;
  if (val > max) val = max;
  return val;
}

export default new Vuex.Store({
  state: {
    name: '',
    balls: initBalls(),
    nballs: 1,
    bpm: 0,
  },
  mutations: {
    UPDATE_NAME(state, name) {
      state.name = name;
    },
    UPDATE_BPM(state, bpm) {
      state.bpm = bpm;
    },
    UPDATE_BALL(state, payload) {
      Vue.set(state.balls[payload.i], payload.prop, payload.value);
    },
    SET_BALL(state, payload) {
      Vue.set(state.balls, payload.i, payload.ball);
    },
    CHANGE_BALL_COUNT(state, delta) {
      state.nballs = clamp(state.nballs + delta, 1, kMaxBalls);
    },
    UPDATE_BALL_COUNT(state, count) {
      state.nballs = count;
    }
  },
});
