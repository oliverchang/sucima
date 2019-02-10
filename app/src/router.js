import Vue from 'vue'
import Router from 'vue-router'
import Controller from './views/Controller.vue'

Vue.use(Router)

export default new Router({
  routes: [
    {
      path: '/',
      redirect: '/controller',
    },
    {
      path: '/controller/:drill?',
      name: 'controller',
      component: Controller
    },
    {
      path: '/drills',
      name: 'drills',
      component: () => import(/* webpackChunkName: "about" */ './views/Drills.vue')
    }
  ]
})
