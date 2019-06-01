<template>
  <div>
    <div class="form-row">
      <div class="d-none d-md-block col-md-6">
        <h1>SUCIMA</h1>
      </div>
      <div class="form-group col-12 col-md-6">
        <div class="btn-group float-right" role="group">
        <div v-if="showInstall">
          <button v-on:click="onClickInstall" id="install" type="button" class="form-control btn btn-primary">Install</button>
        </div>
          <b-button disabled>
          v{{version}}
          </b-button>

          <b-button type="button" class="form-control btn btn-success connect-btn" disabled v-if="connected">
            <i class="material-icons">bluetooth</i>
          </b-button>
          <b-button v-on:click="connectBluetooth" type="button" class="form-control btn btn-warning connect-btn" v-else>
            <i class="material-icons">bluetooth</i>
          </b-button>
        </div>
      </div>
    </div>
    <Controller
        v-bind:connected="connected"
        v-bind:ble-write="writeBuffer"
        v-bind:save-drill="saveDrill">
    </Controller>
  </div>
</template>

<style scoped>
.connect-btn {
  width: 6em;
}
</style>

<script>
import { mapState } from 'vuex'

import Ball from '../ball.js'
import VERSION from '../version.js'
import Controller from '../components/Controller.vue'

export default {
  name: 'controller',
  data() {
    return {
      deferredInstallPrompt: null,
      showInstall: false,
      writeCharacteristic: null,
    };
  },
  mounted() {
    window.addEventListener('beforeinstallprompt', (e) => {
      e.preventDefault();
      this.deferredInstallPrompt = e;
      this.showInstall = true;
    });
  },
  methods: {
    async connectBluetooth() {
      let options = {
        filters: [
          {name: 'Sucima'},
          {services: [0xffe0]},
        ],
      };

      let device = await navigator.bluetooth.requestDevice(options);
      device.addEventListener('gattserverdisconnected', this.onDisconnected);
      await device.gatt.connect();
      let service = await device.gatt.getPrimaryService(0xffe0);
      this.writeCharacteristic = await service.getCharacteristic(0xffe1);
      this.$store.commit('UPDATE_CONNECTED', true);
    },

    onDisconnected() {
      this.$store.commit('UPDATE_CONNECTED', false);
    },

    async writeBuffer(buffer) {
      await this.writeCharacteristic.writeValue(buffer);
    },

    async onClickInstall() {
      this.showInstall = false;
      this.deferredInstallPrompt.prompt();
      await this.deferredInstallPrompt.userChoice();
      this.deferredInstallPrompt = null;
    },

    saveDrill(name, nballs, balls, bpm) {
      let drills;
      if (localStorage.getItem('drills') === null) {
        drills = {};
      } else {
        drills = JSON.parse(localStorage.drills);
      }

      let data = new Uint8Array(2 + nballs * Ball.encodedSize);
      data[0] = bpm;
      data[1] = nballs;
      for (let i = 0; i < nballs; i++) {
        Ball.encode(balls[i], data.subarray(2 + i * Ball.encodedSize));
      }

      drills[name] = btoa(String.fromCharCode(...data));
      localStorage.drills = JSON.stringify(drills);
    },
  },
  components: {
    Controller
  },
  computed: {
    ...mapState([
      'connected',
      'nballs',
    ]),
    version() {
      return VERSION;
    }
  },
}
</script>
