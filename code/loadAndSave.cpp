// loadAndSave.cpp
#include "loadAndSave.h" // 包含对应的头文件
#include <fstream>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>

// 直接在main中创建GameRecord类来存储信息
void GameRecord::setUsername(const std::string &username) { username_ = username; }

void GameRecord::setDuration(double duration) { duration_ = duration; }

void GameRecord::setScore(int score) { score_ = score; }

void GameRecord::setDateTime()
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
    this->dateTime_ = ss.str();
}

void GameRecord::clear()
{
    this->dateTime_ = "";
    this->username_ = defaultUsername;
    this->score_ = 0;
    this->duration_ = 0.0;
}

std::vector<GameRecord> loadGameRecords()
{
    std::vector<GameRecord> records;
    json records_json;

    std::ifstream infile(filename);
    bool flag = false;
    if (infile.is_open())
    {
        try
        {
            infile >> records_json;
            infile.close();
            // 确保是数组类型
            if (records_json.is_array())
            {
                records = records_json.get<std::vector<GameRecord>>();
                std::cout << "成功加载 " << records.size() << " 条游戏记录" << std::endl;
                flag = true;
            }
            else
            {
                std::cerr << "警告: " << filename << " 文件内容不是有效的JSON数组，将视为空记录" << std::endl;
            }
        }
        catch (json::parse_error &e)
        {
            std::cerr << "JSON 解析错误: " << e.what() << " 将视为空记录" << std::endl;
            if (infile.is_open())
                infile.close();
            // 创建空json文件
        }
        catch (json::exception &e)
        {
            std::cerr << "JSON 类型转换错误: " << e.what() << " 将视为空记录" << std::endl;
            if (infile.is_open())
                infile.close();
            ;
        }
    }
    else
    {
        // 文件不存在，这是正常情况，不需要报错，直接返回空vector
        std::cout << "记录文件 " << filename << " 不存在，将创建新文件" << std::endl;
    }
    if (!flag)
    {
        // 创建空json文件
        std::ofstream outfile(filename);
        outfile << json::array().dump(4) << std::endl;
        outfile.close();
    }
    return records;
}

void printGameRecords()
{
    std::vector<GameRecord> records = loadGameRecords();

    if (records.empty())
    {
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "          暂无游戏记录。" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        return;
    }

    // 按分数排序记录 (从高到低)
    std::sort(records.begin(), records.end(), [](const GameRecord &a, const GameRecord &b)
              {
                  return a.getScore() > b.getScore(); // 分数高的排前面
              });

    // 4. 打印表头
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << std::left // 左对齐
              << std::setw(14) << "用户名"
              << std::setw(26) << "游戏时间"
              << std::setw(14) << "时长(秒)"
              << std::setw(10) << "得分" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    // 5. 遍历并打印每条记录
    for (const auto &record : records)
    {
        std::cout << std::left
                  << std::setw(10) << record.getUsername() // 使用 getter 获取私有成员
                  << std::setw(24) << record.getDateTime()
                  << std::setw(10) << std::fixed << std::setprecision(1) << record.getDuration() // 保留一位小数
                  << std::setw(10) << record.getScore() << std::endl;
    }

    std::cout << "-------------------------------------------------------------" << std::endl;
}

void saveGameRecord(const GameRecord &gameRecord)
{

    // 先调用 loadGameRecords 读取现有数据（或得到一个空vector）
    std::vector<GameRecord> existingRecords = loadGameRecords();

    // 将新记录添加到 vector 中
    existingRecords.push_back(gameRecord);

    // 将整个 vector 转换为 JSON 数组
    // nlohmann/json 可以直接从 std::vector<GameRecord> 转换
    json records_json = existingRecords;

    // 将 JSON 数组写入文件（覆盖写入）
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << records_json.dump(4) << std::endl; // 使用格式化输出
        outfile.close();
        std::cout << "游戏记录已更新并保存。" << std::endl;
    }
    else
    {
        std::cerr << "错误: 无法打开文件 " << filename << " 进行写入。" << std::endl;
    }
}