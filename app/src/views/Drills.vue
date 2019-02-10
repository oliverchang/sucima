<template>
  <div>
    <div class="form-row">
      <div class="col-md-12">
        <h1>Drills</h1>
        <b-list-group>
          <b-list-group-item v-for="(value, key) in drills">
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

export default {
  name: 'drills',
  data() {
    return {
      drills: {},
    };
  },
  methods: {
    load(key) {
      this.$router.replace({
        name: 'controller',
        query: {
          name: key,
          drill: this.drills[key],
        }
      });
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
