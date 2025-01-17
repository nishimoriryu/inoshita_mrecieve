/*
 * Copyright (C) 2023 eSOL Co.,Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE FREEBSD PROJECT ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// C++標準ライブラリ
#include <chrono>
#include <functional>
#include <memory>
#include <string>

// ROSのC++クライアントライブラリ
#include "rclcpp/rclcpp.hpp"
// 文字列のメッセージ型
#include "std_msgs/msg/string.hpp"

// 名前空間の省略表記
using namespace std::chrono_literals;

// rclcpp::Nodeを継承したクラスSimpleTalkerの宣言
class SimpleTalker : public rclcpp::Node
{
public:
  // コンストラクタ
  SimpleTalker()
  : Node("simple_talker"), count_(0)   // カウント変数の初期化
  {
    // publisherの生成
    publisher_ = this->create_publisher<std_msgs::msg::String>("chatter", 10);

    // タイマの生成
    timer_ = this->create_wall_timer(500ms, std::bind(&SimpleTalker::timer_callback, this));
  }

private:
  // タイマによって呼び出される関数
  void timer_callback()
  {
    // メッセージを定義し、中身のデータを設定
    std_msgs::msg::String msg;
    msg.data = "hello, world! " + std::to_string(count_);  // カウントをメッセージに追加

    // ターミナルへメッセージ表示
    RCLCPP_INFO(this->get_logger(), "publish: %s", msg.data.c_str());

    // メッセージ送信
    publisher_->publish(msg);

    // カウントアップ
    count_++;
  }

  // 一定周期で処理を実行するタイマ
  rclcpp::TimerBase::SharedPtr timer_;

  // std_msgs::msg::String型のトピックを送信するpublisher
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

  // カウント用の変数
  int count_;
};

int main(int argc, char * argv[])
{
  // rcl(ros client library)の初期化
  rclcpp::init(argc, argv);

  // ノードを作成し、スピン（実行）
  rclcpp::spin(std::make_shared<SimpleTalker>());

  // rclの終了
  rclcpp::shutdown();

  return 0;
}
