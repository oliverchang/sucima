<template>
  <div class="container">
    <keep-alive include="controller">
      <router-view></router-view>
    </keep-alive>
  </div>
</template>

<script>
import Vue from 'vue'
import BootstrapVue from 'bootstrap-vue'
Vue.use(BootstrapVue);

import 'bootswatch/dist/darkly/bootstrap.min.css'
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
