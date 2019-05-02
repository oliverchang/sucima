<template>
<div>
  <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
  <div class="form-row">
    <div class="form-group col-3 col-md-1">
      <div class="btn-group" role="group">
        <button type="button" class="btn btn-danger control" v-on:click="changeBallCount(-1)">
          <i class="material-icons">remove</i>
        </button>
        <button type="button" class="btn btn-primary control" v-on:click="changeBallCount(1)">
          <i class="material-icons">add</i>
        </button>
      </div>
    </div>
    <div class="form-group col-9 col-md-3">
      <div class="btn-group" role="group">
        <b-button v-on:click="navigateBall(-1)" class="btn btn-info">
          <i class="material-icons">navigate_before</i>
        </b-button>
        <b-button v-on:click="navigateBall(1)" class="btn btn-info">
          <i class="material-icons">navigate_next</i>
        </b-button>
        <b-button v-on:click="gotoDrills" class="btn btn-warning">
          <i class="material-icons">sort</i>
        </b-button>
        <b-button v-b-modal.modalSave class="btn btn-success">
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

      <b-button v-on:click="reset" class="btn btn-secondary float-right">
        <i class="material-icons">refresh</i>
      </b-button>
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
      <label for="spin_strength">
        Spin strength
        <input class="val form-control" type="number" v-model="spin_strength">
      </label>
      <input class="form-control custom-range" v-model="spin_strength" id="spin_strength" type="range" min="0" max="20">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="spin_angle">
        Spin angle
        <input v-bind:disabled="spinDisabled" class="val form-control" type="number" v-model="spin_angle">
        <small class="spin-desc">{{spinDesc}}</small>
      </label>
      <input v-bind:disabled="spinDisabled" class="form-control custom-range" v-model="spin_angle" id="spin" type="range" min="-180" max="180" step="2">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="speed">
        Speed
        <input class="val form-control" type="number" v-model="speed">
      </label>
      <input class="form-control custom-range" v-model="speed" id="speed" type="range" min="1" max="20">
    </div>
  </div>

  <div class="form-row">
    <div class="form-group col-12 col-md-4">
      <label for="position">
        Position
        <input class="val form-control" type="number" v-model="position">
      </label>
      <input class="form-control custom-range" v-model="position" id="position" type="range" min="-8" max="8">
    </div>

    <div class="form-group col-12 col-md-4">
      <label for="trajectory">
        Trajectory
        <input class="val form-control" type="number" v-model="trajectory">
      </label>
      <input class="form-control custom-range" v-model="trajectory" id="trajectory" type="range" min="0" max="16">
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
import { mapState } from 'vuex'
import Ball from '../ball.js'

function encodeString(str, result) {
  for (let i = 0; i < str.length; i++) {
    result[i] = str.charCodeAt(i);
  }
}

function computedBallProperties() {
  const properties = [
    'spin_strength',
    'spin_angle',
    'speed',
    'position',
    'trajectory',
  ];

  let result = {};
  for (let prop of properties) {
    result[prop] = {
      get() {
        return this.$store.state.balls[this.cur-1][prop];
      },
      set(value) {
        this.$store.commit('UPDATE_BALL', {
          i: this.cur-1,
          prop: prop,
          value: value,  
        });
      },
    };
  }

  return result;
}

export default {
  name: 'Controller',
  data() {
    return {
      cur: 1,
    };
  },
  props: {
    bleWrite: Function,
    saveDrill: Function,
  },
  mounted() {
  },
  methods: {
    changeBallCount(delta) {
      this.$store.commit('CHANGE_BALL_COUNT', delta);
      if (this.cur > this.nballs)
        this.cur = this.nballs;
    },
    navigateBall(delta) {
      this.cur = Math.min(Math.max(this.cur + delta, 1), this.nballs);
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
    },

    reset() {
      this.$store.commit('UPDATE_NAME', '');
      this.$store.commit('UPDATE_BPM', 0);
      this.$store.commit('UPDATE_BALL_COUNT', 1);
      this.$store.commit('RESET_BALLS');
    }
  },
  computed: {
    saveDisabled() {
      return this.name.length == 0;
    },
    startDisabled() {
      return !this.connected || this.bpm === 0;
    },
    spinDisabled() {
      return this.balls[this.cur-1].spin_strength == 0;
    },
    spinDesc() {
      if (this.spinDisabled) return '';

      if (this.spin_angle == 0) {
        return 'Top';
      } else if (this.spin_angle == -180 || this.spin_angle == 180) {
        return 'Back';
      } else if (this.spin_angle == 90) {
        return 'Right';
      } else if (this.spin_angle == -90) {
        return 'Left';
      } else if (this.spin_angle > -180 && this.spin_angle < -90) {
        return 'Bottom left';
      } else if (this.spin_angle > -90 && this.spin_angle < 0) {
        return 'Top left';
      } else if (this.spin_angle > 0 && this.spin_angle < 90) {
        return 'Top right';
      } else if (this.spin_angle > 90 && this.spin_angle < 180) {
        return 'Bottom left';
      }

      return '';
    },
    bpm: {
      get() {
        return this.$store.state.bpm;
      },
      set(value) {
        this.$store.commit('UPDATE_BPM', value);
      },
    },
    name: {
      get() {
        return this.$store.state.name;
      },
      set(value) {
        this.$store.commit('UPDATE_NAME', value);
      },
    },
    ...computedBallProperties(),
    ...mapState([
      'connected',
      'balls',
      'nballs',
    ])
  },
}
</script>

<style scoped>
  .val-2 {
    max-width: 3em;
    display: inline;
  }
  .val {
    max-width: 4em;
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
  .spin-desc {
    padding-left: 1em;
  }
</style>
