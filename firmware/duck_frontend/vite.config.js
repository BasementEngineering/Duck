import { defineConfig } from 'vite'
import { viteSingleFile } from "vite-plugin-singlefile"
import viteCompression from 'vite-plugin-compression'
import PurgeIcons from 'vite-plugin-purge-icons'

export default defineConfig({
    plugins: [
      PurgeIcons({}),
    ],//viteSingleFile()],//,viteCompression()],
    publicDir: "public",
    /*resolve: {
      alias: {
        '~icons': 'node_modules/@iconify-json',
      }
    },*/  
    server:{
      hmr:
      {overlay: false}},
    assetsInclude: [],
    esbuild: {
        minify: true,
        minifySyntax: true
      }
 })