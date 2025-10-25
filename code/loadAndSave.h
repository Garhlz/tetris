// loadAndSave.h
#ifndef LOAD_AND_SAVE_H // 防止头文件被重复包含的Include Guard
#define LOAD_AND_SAVE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp> // 包含json库头文件
// 使用json命名空间
using json = nlohmann::json;

const std::string filename = "game_records.json";
const std::string defaultUsername = "Guest";
class GameRecord
{
public:
    GameRecord() : username_(defaultUsername), dateTime_(""), duration_(0), score_(0) {}
    GameRecord(const std::string &username, const std::string &dateTime, double duration, int score) : username_(username), dateTime_(dateTime), duration_(duration), score_(score) {}
    void setUsername(const std::string &username);
    void setDateTime();
    void setDuration(double);
    void setScore(int);

    std::string getUsername() const { return username_; }
    std::string getDateTime() const { return dateTime_; }
    double getDuration() const { return duration_; }
    int getScore() const { return score_; }

    void clear();

private:
    std::string username_;
    std::string dateTime_;
    double duration_;
    int score_;

    // 声明宏，以便在.cpp文件中定义转换规则
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameRecord, username_, dateTime_, duration_, score_);
};

/**
 * @brief 从 "game_records.json" 文件加载游戏记录。
 * 如果文件不存在或内容无效，则返回一个空的vector并打印提示。
 * @return 包含所有已加载游戏记录的 vector。
 */
std::vector<GameRecord> loadGameRecords();

/**
 * @brief 打印游戏记录
 */
void printGameRecords();

/**
 * @brief 将一条新的游戏记录追加到 "game_records.json" 文件。
 * 会先读取现有记录，添加新记录，然后覆盖写入文件。
 * @param user 用户名。
 * @param gameDuration 游戏时长（秒）。
 * @param finalScore 最终得分。
 */
void saveGameRecord(const GameRecord &gameRecord);

#endif // LOAD_AND_SAVE_H