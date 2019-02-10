<template>
  <div class="container">
    <router-view></router-view>
  </div>
</template>

<script>
import Vue from 'vue'
import BootstrapVue from 'bootstrap-vue'
Vue.use(BootstrapVue);

import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap-vue/dist/bootstrap-vue.css'

export default {
  name: 'app',
}
</script>

<style>

body {
  overscroll-behavior-y: contain;
}

.container {
  padding-top: 2em;
}

</style>
