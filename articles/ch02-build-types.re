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
 ** labelに@<b>{${appName\}}の記述を追加
 * build.gradle

//source[AndroidManifest.xml]{
<?xml version="1.0" encoding="utf-8"?>
<manifest>
  ....
  <application
    android:label="${appName}">
    ....
  </application>
</manifest>
//}

//source[build.gradle]{
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
