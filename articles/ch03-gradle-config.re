= gradle-configを使って環境ごとの向き先を振り分ける

== 概要
開発環境や本番環境の向き先を変える時、どういった方法で実装するのがよいだろうか。

人によってはbuild.gradle経由でBuildConfigを定義する人もいるだろうか、場合によっては独自に実装している人もいるかもしれない。

それぞれメリット・デメリットありそうだが、今回は第三の選択肢になり得そうなgradle-config@<fn>{gradle-config}を紹介しようと思う。

== gradle-configの特長
 * APIの向き先などの設定をyamlで書ける
 * 開発向け・本番向けなどProductFlavorを用いて細かく分けることができる
 * トークンなど外に公開したくない情報は別管理できる
 * yamlに記述した値を参照できるjavaクラスを自動生成してくれる

BuildConfigによる方法はbuild.gradleに上に記述するが、gradle-configはyamlとして切り出して管理できるため肥大化を防ぐことができる。

== gradle-configの導入方法
導入は簡単で、アプリのbuild.gradleにgradle-configを使うための記述をすれば良い。

//list[gradle-config-install][build.script]{
apply plugin: 'com.android.application'
apply plugin: 'com.tmiyamon.config'

buildscript {
  repositories {
    maven { url 'https://plugins.gradle.org/m2/'}
  }
  dependencies {
    classpath 'gradle.plugin.com.tmiyamon:gradle-config:0.2.1'
  }
}
//}

これで導入は完了したので、あとは設定ファイルを書いていくだけである。

== yamlファイルへの記述
プロジェクトのルートディレクトリに@<b>{config}ディレクトリを作る。

その下に@<b>{default.yml}というファイルを作ろう。

少し分かりづらい人のために簡単な階層図も書いておく。

//list[gradle-config-packages][ここまでの階層構造]{
- root
  - app
    - build.gradle
  - build.gradle
  - config(*これが今回追加したディレクトリ*)
    - default.yml
  - gradlew
  - settings.gradle
  ....
//}

default.ymlは全てのビルドで共通で見る値を管理する。

例えばアプリ上で定数として扱うような値をここで定義した場合はこのようになる。

//list[gradle-config-default-yml][default.yml]{
app:
  helloWorld: 'Hello world!!'
  hoge:
   - foo
   - bar
  piyo: true
api:
  defaultOffset: 0
  defaultLimit: 20
//}

この記述をすると以下のような感じでSettingsという名前のクラスを自動生成してくれて参照できるようになる。

//cmd{
Settings.app.helloWorld // Hello world!!
Settings.app.hoge[0] // foo
Settings.app.hoge[1] // bar
Settings.app.piyo // true
Settings.api.defaultOffset // 0
Settings.api.defaultLimit // 20
//}

型はyamlに設定した値から自動的に解決してくれるので便利。

このような感じで参照するためのクラスも自動生成してくれるので自分で定義する必要がない。

またyamlも読みやすいので管理しやすいのが良いと思う。

== ProductFlavor単位で設定を分ける
APIの向き先あたりはProductFlavor単位で分けたくなる。

このプラグインはそういった出し分けの機能も提供してくれているので、それに乗っかれば簡単に実現できる。

=== コンフィグファイルの追加
{productFlavor}.ymlというファイル名で追加すれば良い。

=== 公開したくない設定を記述したい
@<b>{default_secret.yml}といった感じでファイル名の末尾に@<b>{_secret}を付ければ良い。

もしこのような設定項目がある場合は、.gitignoreに@<b>{*_secret.yml}を追加してgit管理上に載せないようにしよう。

//footnote[gradle-config][https://github.com/tmiyamon/gradle-config]
