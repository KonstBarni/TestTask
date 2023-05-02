#ifndef SIMPLE_PARSER_HPP
#define SIMPLE_PARSER_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

class Packet
{
public:
    virtual std::string to_string() const = 0;
};

class Comand : virtual public Packet            // класс команд
{
private:
    std::string type;                           //тип команды
    std::vector<uint8_t>cmd;                    //вектор сщманды

public:
    Comand(std::vector<uint8_t>inputData):cmd(inputData){}

    std::string to_string() const override;                 //метод для вывода команды в строку
    void set_type();
    
    ~Comand(){}
};

class SimpleParser
{
public:
    SimpleParser(){}

    void push(uint8_t b);
    void reset();

    std::shared_ptr<Packet> get_packet() const;

private:

    bool start = false;                         //флаг старта записи пакета
    std::vector<Comand>comand{};                //вектор класса соманд
    std::vector<uint8_t>inputData{};            //вектор входящих данных
    std::shared_ptr<Comand>packet = nullptr;    //указатель на объект команды
};

#endif // SIMPLE_PARSER_HPP