<template>
<div>
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
  <div class="form-row">
    <div class="form-group col-3 col-md-1">
      <div class="btn-group" role="group">
        <button type="button" class="btn btn-danger" v-on:click="changeBallCount(-1)">
          <i class="material-icons">remove</i>
        </button>
        <button type="button" class="btn btn-primary" v-on:click="changeBallCount(1)">
          <i class="material-icons">add</i>
        </button>
      </div>
    </div>
    <div class="form-group col-9 col-md-3">
      <div class="btn-group" role="group">
        <button v-on:click="navigateBall(-1)" type="button" class="btn btn-info">
          <i class="material-icons">navigate_before</i>
        </button>
        <button v-on:click="navigateBall(1)" type="button" class="btn btn-info">
          <i class="material-icons">navigate_next</i>
        </button>
        <button v-on:click="gotoDrills" type="button" class="btn btn-warning">
          <i class="material-icons">sort</i>
        </button>
        <b-button v-b-modal.modalSave type="button" class="btn btn-success">
          <i class="material-icons">save</i>
        </b-button>
        <b-modal
          id="modalSave"
          ref="modal"
          title="Save drill"
          v-bind:ok-disabled="saveDisabled"
          @ok="save">
          <form @submit.stop.prevent="handleSubmit">
            <b-form-input type="text" placeholder="Drill name" v-model="name" />
          </form>
        </b-modal>
      </div>
    </div>
    <div class="form-group col-12 col-md-4">
      Ball
      <input class="val-2 form-control" type="number" v-model="cur">
      /
      <input class="val-2 form-control" type="number" v-model="nballs" readonly>
    </div>
  </div>
  <div class="form-row">
    <div class="form-group col-12 col-md-4">
      <label for="spin_angle">
        Spin angle
        <input class="val form-control" type="number" v-model="balls[cur-1].spin_angle">
      </label>
      <input class="form-control custom-range" v-model="balls[cur-1].spin_angle" id="spin" type="range" min="0" max="360" step="2">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="spin_strength">
        Spin strength
        <input class="val form-control" type="number" v-model="balls[cur-1].spin_strength">
      </label>
      <input class="form-control custom-range" v-model="balls[cur-1].spin_strength" id="spin_strength" type="range" min="0" max="20">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="speed">
        Speed
        <input class="val form-control" type="number" v-model="balls[cur-1].speed">
      </label>
      <input class="form-control custom-range" v-model="balls[cur-1].speed" id="speed" type="range" min="1" max="20">
    </div>
  </div>

  <div class="form-row">
    <div class="form-group col-12 col-md-4">
      <label for="position">
        Position
        <input class="val form-control" type="number" v-model="balls[cur-1].position">
      </label>
      <input class="form-control custom-range" v-model="balls[cur-1].position" id="position" type="range" min="-8" max="8">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="trajectory">
        Trajectory
        <input class="val form-control" type="number" v-model="balls[cur-1].trajectory">
      </label>
      <input class="form-control custom-range" v-model="balls[cur-1].trajectory" id="trajectory" type="range" min="0" max="16">
    </div>
  </div>
  <div class="form-row">
    <div class="d-none d-md-block col-md-4">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="bpm">
        Balls/min
      <input class="val form-control" type="number" v-model="bpm">
      </label>
      <input class="form-control custom-range" v-model="bpm" id="bpm" type="range" min="0" max="100" value="0">
    </div>
    <div class="col-12 col-md-4">
      <div class="action-group btn-group" role="group">
        <button v-bind:disabled="!connected" v-on:click="sample" type="button" class="action btn btn-info">Sample</button>
        <button v-bind:disabled="startDisabled" v-on:click="start" type="button" class="action btn btn-primary">Start</button>
        <button v-bind:disabled="!connected" v-on:click="setBpm(0)" type="button" class="action btn btn-danger">Stop</button>
      </div>
    </div>
  </div>
  </div>
</template>

<script>
import Ball from '../ball.js'

const kMaxBalls = 8;

function clamp(val, min, max) {
  if (val < min) val = min;
  if (val > max) val = max;
  return val;
}

function encodeString(str, result) {
  for (let i = 0; i < str.length; i++) {
    result[i] = str.charCodeAt(i);
  }
}

function initBalls(initialBalls) {
  let balls = new Array(kMaxBalls);
  for (let i = 0; i < kMaxBalls; i++) {
    if (initialBalls && i < initialBalls.length) {
      balls[i] = initialBalls[i];
    } else {
      balls[i] = new Ball(0, 0, 0, 0, 1);
    }
  }

  return balls;
}

export default {
  name: 'Controller',
  data() {
    return {
      bpm: this.initialBpm || 0,
      cur: 1,
      balls: initBalls(this.initialBalls),
      nballs: this.initialNumBalls || 1,
      name: this.initialName || '',
    };
  },
  props: {
    initialBalls: Array,
    initialName: String,
    initialNumBalls: Number,
    initialBpm: Number,
    bleWrite: Function,
    connected: Boolean,
    saveDrill: Function,
  },
  mounted() {
    //if (this.initialBalls.length)
    //  this.balls = this.initialBalls;
  },
  methods: {
    changeBallCount(delta) {
      this.nballs = clamp(this.nballs + delta, 1, kMaxBalls);
      if (this.cur > this.nballs)
        this.cur = this.nballs;
    },
    navigateBall(delta) {
      this.cur = clamp(this.cur + delta, 1, this.nballs);
    },
    async sample() {
      if (!this.connected) return;

      let command = 'SMPL';
      let array = new Int8Array(command.length + Ball.encodedSize);

      encodeString(command, array);
      this.balls[this.cur - 1].encode(array.subarray(command.length))

      await this.bleWrite(array.buffer);
    },

    async start() {
      if (!this.connected) return;

      let command = 'SET';
      let array = new Int8Array(command.length + 1 + Ball.encodedSize * this.nballs);
      encodeString(command, array);
      array[command.length] = this.nballs;

      for (let i = 0; i < this.nballs; i++) {
        this.balls[i].encode(array.subarray(command.length + 1 + Ball.encodedSize * i));
      }

      await this.bleWrite(array.buffer);
      await this.setBpm(this.bpm);
    },

    async setBpm(bpm) {
      if (!this.connected) return;

      let command = 'SBPM';
      let array = new Int8Array(command.length + 1);
      encodeString(command, array);

      array[command.length] = bpm;
      await this.bleWrite(array.buffer);
    },

    save() {
      this.saveDrill(this.name, this.nballs, this.balls, this.bpm);
    },

    gotoDrills() {
      this.$router.push('/drills');
    }
  },
  watch: {
    initialBalls() {
      this.balls = initBalls(this.initialBalls);
    },
    initialName() {
      this.name = this.initialName;
    },
    initialNumBalls() {
      this.nballs = this.initialNumBalls;
    },
    initialBpm() {
      this.bpm = this.initialBpm;
    }
  },
  computed: {
    saveDisabled() {
      return this.name.length == 0;
    },
    startDisabled() {
      return !this.connected || this.bpm === 0;
    },
  },
}
</script>

<style scoped>
  .val-2 {
    max-width: 3em;
    display: inline;
  }
  .val {
    max-width: 3.5em;
    display: inline;
  }
  .action-group {
    width: 100%;
    height: 100%;
  }
  .action {
    width: 33%;
  }
  .material-icons {
    font-size: 12px;
  }
  input[type="number"]::-webkit-outer-spin-button,
  input[type="number"]::-webkit-inner-spin-button {
      -webkit-appearance: none;
      margin: 0;
  }
  input[type="number"] {
      -moz-appearance: textfield;
  }
</style>
