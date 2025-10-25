// loadAndSave.cpp
#include "loadAndSave.h" // ������Ӧ��ͷ�ļ�
#include <fstream>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <sstream>

// ֱ����main�д���GameRecord�����洢��Ϣ
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
            // ȷ������������
            if (records_json.is_array())
            {
                records = records_json.get<std::vector<GameRecord>>();
                std::cout << "�ɹ����� " << records.size() << " ����Ϸ��¼" << std::endl;
                flag = true;
            }
            else
            {
                std::cerr << "����: " << filename << " �ļ����ݲ�����Ч��JSON���飬����Ϊ�ռ�¼" << std::endl;
            }
        }
        catch (json::parse_error &e)
        {
            std::cerr << "JSON ��������: " << e.what() << " ����Ϊ�ռ�¼" << std::endl;
            if (infile.is_open())
                infile.close();
            // ������json�ļ�
        }
        catch (json::exception &e)
        {
            std::cerr << "JSON ����ת������: " << e.what() << " ����Ϊ�ռ�¼" << std::endl;
            if (infile.is_open())
                infile.close();
            ;
        }
    }
    else
    {
        // �ļ������ڣ������������������Ҫ����ֱ�ӷ��ؿ�vector
        std::cout << "��¼�ļ� " << filename << " �����ڣ����������ļ�" << std::endl;
    }
    if (!flag)
    {
        // ������json�ļ�
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
        std::cout << "          ������Ϸ��¼��" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        return;
    }

    // �����������¼ (�Ӹߵ���)
    std::sort(records.begin(), records.end(), [](const GameRecord &a, const GameRecord &b)
              {
                  return a.getScore() > b.getScore(); // �����ߵ���ǰ��
              });

    // ��ӡ��ͷ
    std::cout << "-------------------------------------------------------------" << std::endl;
    std::cout << std::left // �����
              << std::setw(14) << "�û���"
              << std::setw(26) << "��Ϸʱ��"
              << std::setw(14) << "ʱ��(��)"
              << std::setw(10) << "�÷�" << std::endl;
    std::cout << "-------------------------------------------------------------" << std::endl;

    // ��������ӡÿ����¼
    for (const auto &record : records)
    {
        std::cout << std::left
                  << std::setw(14) << record.getUsername() // ʹ�� getter ��ȡ˽�г�Ա
                  << std::setw(26) << record.getDateTime()
                  << std::setw(14) << std::fixed << std::setprecision(1) << record.getDuration() // ����һλС��
                  << std::setw(10) << record.getScore() << std::endl;
    }

    std::cout << "-------------------------------------------------------------" << std::endl;
}

void saveGameRecord(const GameRecord &gameRecord)
{

    // �ȵ��� loadGameRecords ��ȡ�������ݣ���õ�һ����vector��
    std::vector<GameRecord> existingRecords = loadGameRecords();

    // ���¼�¼��ӵ� vector ��
    existingRecords.push_back(gameRecord);

    // ������ vector ת��Ϊ JSON ����
    // nlohmann/json ����ֱ�Ӵ� std::vector<GameRecord> ת��
    json records_json = existingRecords;

    // �� JSON ����д���ļ�������д�룩
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        outfile << records_json.dump(4) << std::endl; // ʹ�ø�ʽ�����
        outfile.close();
        std::cout << "��Ϸ��¼�Ѹ��²����档" << std::endl;
    }
    else
    {
        std::cerr << "����: �޷����ļ� " << filename << " ����д�롣" << std::endl;
    }
}