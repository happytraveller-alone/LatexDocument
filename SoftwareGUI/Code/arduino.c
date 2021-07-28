// 针脚定义
const int stepPin = 6;        // PUL -Pulse
const int dirPin = 7;         // DIR -Direction
const int enPin = 8;          // ENA -Enable
const byte ledPin = 4;        // LED -Interrupt
const byte interruptPin = 2;  // KEY - Interrupt
volatile byte state = LOW;    // 定义默认输入状态
int reset_num = 0;            // 电机转数

void setup() {
    // 针脚设置
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(interruptPin, INPUT_PULLUP);
    // 启动使能端
    digitalWrite(enPin, LOW);
    // 串口通信初始化
    Serial.begin(9600);
    // 监视中断输入引脚的变化
    attachInterrupt(digitalPinToInterrupt(interruptPin), stateChange, HIGH);
    // 串口打印转数数据
    Serial.println(reset_num);
    // 延迟确保通电后数据为0
    delay(3000);
}

void loop() {
    // 循环结构内部，设置正向函数
    // 从靠近电机端移动到远离电机端
    // 默认LED提示灯为熄灭状态
    state = LOW;
    digitalWrite(ledPin, state);
    digitalWrite(dirPin, LOW);
    while (reset_num <= 20) {
        for (int x = 0; x < 200; x++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(320);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(320);
        }
        reset_num++;
        Serial.println(reset_num);
    }
}

void stateChange() {
    // 设置限位开关模拟
    // 触碰开关后，进入中断程序
    // 从远离电机端移动到靠近电机端
    // 移动过程中，LED显示灯长亮
    state = HIGH;
    digitalWrite(ledPin, state);
    digitalWrite(dirPin, HIGH);
    while (reset_num >= 2) {
        for (int x = 0; x < 200; x++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(320);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(320);
        }
        reset_num--;
        Serial.println(reset_num);
    }
    state = HIGH;
    // 普通的延迟函数实质上是中断函数
    // 由于函数中断优先级高于时间中断，故无法执行
    for (int i = 0; i < 60; i++) {
        delayMicroseconds(10000);
    }
}
