= DeployGateを使って開発中のアプリを企画やデザイナーに見せる
アプリの開発が進んでくると、開発途中の成果を他の人に見てもらいたい場合がある。

特に会社のプロダクト開発では企画やデザイナーに動作確認の過程で見せることが多いのではないだろうか。

その時に企画やデザイナーにAndroidの開発環境を用意するのは現実的ではないし、こちら側で都度APKを作ってどこかにアップロードするのは手間がかかる。

そのような問題を解消してくれるのがDeployGate@<fn>{deploygate}である。

== DeployGateの特長
開発中のアプリをβ配信に特化したプラットフォーム。

元はmixiの開発者によって作られたサービスだが、現在はDeployGate社として独立して事業譲渡が行われている。

企画やデザイナーに使ってもらうためには如何に簡単な手順で確認環境を構築することと、追従を楽にすることが必要不可欠だが、DeployGateはそれらを比較的簡単に解決してくれる。

=== 企画・デザイナー側の導入が簡単
QRコードを読んだ後に画面に沿って操作を進めていくだけで確認環境が整う。

β配信のAPKが変更された時も通知が飛ぶので、通知を押せば対象のAPKを更新することができる。

APKに何らかの問題が発生した場合はログが送信されているので、原因を容易に特定することが可能になっている。

=== Gradle Pluginを用いてAPKを簡単に更新できる
DeployGateから公式のGradle Pluginが提供されている。

これを使うと、管理画面からAPIキーを発行してbuild.gradleに記述すればBuild Variantsのタスクが自動生成される。

あとはタスクを実行するだけでAPKの更新ができるようになる。

=== Slack連携がサポートされている
もし開発用のコミュニケーションツールでSlackを採用しているのであれば、APKが更新された時に指定のチャンネルに発言してくれる連携を利用できる。

リンク先は配布用ページなので、関係者が自分で配布用ページからβ配信の動作確認が行えるようになっている。

== DeployGateの導入方法
アカウント作成については割愛する。

Gradle Plugin経由からβ配信するためのAPIキーは@<b>{アカウント設定}から行える。

=== 自分のプロダクトに組み込む
AndroidでDeployGateを使うならDeployGate Gradle Plugin@<fn>{deploygate-plugin}を使うと簡単に導入できる。

今回もDeployGate Gradle Pluginを用いて説明する。

まずはプロジェクトルートのbuild.gradleの@<b>{buildscript}の項目にDeployGate Gradle Pluginのクラスパスを通す。

//list[deploygate-plugin-root][build.gradle]{
buildscript {
  repositories {
    jcenter()
  }
  dependencies {
    classpath 'com.deploygate:gradle:1.1.0'
  }
}
//}

プラグインのバージョンはこれより新しくなっている場合もあるので、Githubのリポジトリを確認して適宜変更すること。

その後はモジュールのbuild.gradleにプラグインの適用と設定を記述していく。

//list[deploygate-plugin-module][build.gradle]{
apply plugin: 'deploygate'

deploygate {
  userName = "DeployGateのユーザー名"
  token = "アカウント情報で確認できるAPIトークン"
  apks {
    debug {
      distributionKey = "DeployGateの配布ページなどで確認できるAPKのアップロード先のキー"
      sourceFile = file("アップロードするAPKのファイルパス")
    }
  }
}
//}

@<b>{apks}の部分はBuild Variantsを指定して細かく設定を書ける。

=== APKをDeployGateにアップロードする
ここまで準備ができたら実際にAPKをアップロードしてみよう。

先ほどのbuild.gradleへの記述でGradleのタスクが増えているので、それを実行するだけでアップロードができる。

//cmd{
./gradlew uploadDeployGateDebug

// 前の章などでProductFlavorを定義していた場合
./gradlew uploadDeployGateDevelopDebug
//}

//footnote[deploygate][https://deploygate.com]
//footnote[deploygate-plugin][https://github.com/DeployGate/gradle-deploygate-plugin]
