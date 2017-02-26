= ビルドごとに作られたAPKを端末内で見分けやすくする

== 概要
@<chapref>{ch01-build-variants}でビルドタイプごとに同じ端末内にインストールできる仕組みを説明した。

ここである問題が起こる。アプリのアイコンが一緒なので同じ端末内に複数のビルドタイプのAPKをインストールしてしまうと見分けが付かなくなってしまう。

この問題を放置すると確認作業の妨げになるので、視覚的に見分けがつくような工夫を図りたい。

== 解決方法
基本的にはAndroidホーム画面のアプリ一覧でどのビルドのアプリであるかひと目で分かる要素があれば良い。

今回用意してあるアプローチは２つある。

=== Manifest Placeholderを用いてアプリのタイトルで分ける
Android Gradle PluginにはAndroidManifest.xmlにプレースホルダーを設定できる仕組みが用意されている。

Manifest PlaceholderはAndroidManifest.xml内にあるプレースホルダーに入れる文字をbuild.gradleから指定できる機能である。

これを利用することで、ビルドタイプごとにアプリのタイトルやAndroidManifest.xmlに入れるメタ情報の切り替えを実現できる。

==== Manifest Placeholderを使ってタイトルを設定する
今回は２つのファイルに手を加える。

 * AndroidManifest.xml
 * build.gradle

//list[placeholder-manifest][AndroidManifest.xml]{
<?xml version="1.0" encoding="utf-8"?>
<manifest>
  ....
  <application
    android:label="${appName}">
    ....
  </application>
</manifest>
//}

//list[placeholder-build][build.gradle]{
android {
  ....
  productFlavors {
    develop {
      manifestPlaceholders = [appName: "[開発]私のアプリ"]
    }
    production {
      manifestPlaceholders = [appName: "私のアプリ"]
    }
  }
}
//}

=== Gradle Pluginを用いて色付きのリボンで分ける
gradle-android-ribbonizer-plugin@<fn>{ribbonizer-plugin}というGradle Pluginを用いることで、ビルド時にアプリのアイコンに処理を加えてリボンを付けるライブラリがある。

使い方も非常に簡単でpluginを有効化するだけで自動的にリボンを付けてくれるようになる。

//list[plugin-build][build.gradle]{
apply plugin: 'com.android.application'
apply plugin: 'com.github.gfx.ribbonizer'
....
buildscript {
  dependencies {
    classpath 'com.android.tools.build:gradle:2.2.3'
    classpath 'com.github.gfx.ribbonizer:plugin:1.0.0'
  }
}
android {
  buildTypes {
    debug { /* 自動的にリボンが付く */ }
    release { /* リボンが付かない */ }
  }
}
//}

これをデバッグビルドで実行した後、端末にインストールするとこのような感じでアイコンを表示してくれる。

//image[1][リボンがついて見分けやすくなった]{
//}

====[column] Product Flavors毎にリボンを付ける

build.gradle内に@<b>{ribbonizer}の項目を追加すれば細かい設定を定義できる。

//list[ribbonizer-option][build.gradle]{
android {
  ....
}
ribbonizer {
  builder { variant, iconFile ->
    if (variant.flavorName == "develop") {
        return yellowRibbonFilter(variant, iconFile)
    } else {
      return greenRibbonFilter(variant, iconFile)
    }
  }
}
//}

developのProduction Flavorの時は黄色のリボンで表示する。

それ以外のデバッグビルドは緑色のリボンで表示されるようになる。

このような感じで細かく色も指定することで、より見分けが付きやすくなるので是非利用していきたい。

====[/column]


//footnote[ribbonizer-plugin][https://github.com/gfx/gradle-android-ribbonizer-plugin]
