module.exports = {
  pwa: {
    name: 'Sucima',
    themeColor: '#ffffff',
    msTileColor: '#ffffff',
    appleMobileWebAppCapable: 'yes',
    appleMobileWebAppStatusBarStyle: 'black',

    // configure the workbox plugin
    workboxPluginMode: 'GenerateSW',
    workboxOptions: {
      exclude: [
        /\.map$/,
        /^manifest.*\.js(?:on)?$/,
        /robots\.txt$/,
        /_headers$/
      ],
    },
  }
}
