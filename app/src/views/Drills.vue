<template>
  <div>
    <div class="form-row">
      <div class="col-md-12">
        <h1>Drills</h1>
        <b-list-group>
          <b-list-group-item v-for="(value, key) in drills" v-bind:key="key">
            {{ key }}
            <b-button-group class="float-right">
              <b-button v-on:click="load(key)" variant="primary">Load</b-button>
              <b-button v-on:click="del(key)" variant="danger">Delete</b-button>
            </b-button-group>
          </b-list-group-item>
        </b-list-group>
      </div>
    </div>
  </div>
</template>
<script>
import Ball from '../ball.js'

export default {
  name: 'drills',
  data() {
    return {
      drills: {},
    };
  },
  methods: {
    load(key) {
      let data;
      try {
        data = Uint8Array.from(atob(this.drills[key]), c => c.charCodeAt(0));
      } catch (e) {
        console.log('Failed to decode.');
        return;
      }

      this.$store.commit('UPDATE_BPM', data[0]);
      this.$store.commit('UPDATE_BALL_COUNT', data[1]);
      for (let i = 0; i < data[1]; i++) {
        let index = 2  + i * Ball.encodedSize;
        this.$store.commit('SET_BALL', {
          i: i,
          ball: Ball.decode(data.slice(index, index + Ball.encodedSize).buffer),
        });
      }

      this.$store.commit('UPDATE_NAME', key);
      this.$router.back();
    },
    del(key) {
      this.$delete(this.drills, key);
      localStorage.drills = JSON.stringify(this.drills);
    }
  },
  mounted() {
    if (!localStorage.getItem('drills'))
      return;

    this.drills = JSON.parse(localStorage.drills);
  }
}
</script>
