// loadAndSave.h
#ifndef LOAD_AND_SAVE_H // ��ֹͷ�ļ����ظ�������Include Guard
#define LOAD_AND_SAVE_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp> // ����json��ͷ�ļ�
// ʹ��json�����ռ�
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

    // �����꣬�Ա���.cpp�ļ��ж���ת������
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(GameRecord, username_, dateTime_, duration_, score_);
};

/**
 * @brief �� "game_records.json" �ļ�������Ϸ��¼��
 * ����ļ������ڻ�������Ч���򷵻�һ���յ�vector����ӡ��ʾ��
 * @return ���������Ѽ�����Ϸ��¼�� vector��
 */
std::vector<GameRecord> loadGameRecords();

/**
 * @brief ��ӡ��Ϸ��¼
 */
void printGameRecords();

/**
 * @brief ��һ���µ���Ϸ��¼׷�ӵ� "game_records.json" �ļ���
 * ���ȶ�ȡ���м�¼������¼�¼��Ȼ�󸲸�д���ļ���
 * @param user �û�����
 * @param gameDuration ��Ϸʱ�����룩��
 * @param finalScore ���յ÷֡�
 */
void saveGameRecord(const GameRecord &gameRecord);

#endif // LOAD_AND_SAVE_H