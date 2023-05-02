#include "simple_parser.hpp"

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

uint8_t getchecksum(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
{
    int whole_checksum = 0;
    uint8_t checksum;

    std::for_each(begin, end, [&whole_checksum, &checksum] (uint8_t n) {
        whole_checksum = whole_checksum + n;
        checksum = ~(0xFF & whole_checksum) + 1;
    });

    return checksum;
}   


void SimpleParser::push(uint8_t b){
    uint8_t check = getchecksum(inputData.cbegin(), inputData.cend());  //контр. сумму для проверки

    if(start)                      
        inputData.push_back(b);     //если старт, принимает данные

    if(check == b && start){  
        Comand com = Comand(inputData);              //если контр.суммы совпали, создает объект команды
        packet = std::make_shared<Comand>(com);
        com.set_type();
        comand.push_back(com);
        
    }

    if(b == 0x53){
        start = true;                       //отлавливает стартовый битб, ставид флаг старт
        inputData.push_back(b);
    }        
}

void SimpleParser::reset(){
    comand.clear();
    inputData.clear();
    packet = nullptr;
    start = false;   
}

void Comand::set_type(){                    //ставит тип команды

    for(auto byte: cmd){

        if(byte == 0x60)
            type = "POLL";

        if(byte == 0x6A)
            type = "BUZ";
    }
}

std::shared_ptr<Packet> SimpleParser::get_packet() const {

    return packet;
}

std::string Comand::to_string()const{       //метод в перевода данных в строку

    int addr{0}, sqn{0}, reader{0}, tone{0}, on{0}, off{0}, count{0};
    std::string to_str, s_type ,s_addr, s_sqn, s_reader, s_tone, s_on, s_off, s_count;

    addr = (int)cmd.at(1);
    sqn = (int)cmd.at(4);

    std::stringstream ss;           //переводит через поток в string

    ss << addr;                     //принимает int отдает string
    ss >> s_addr; ss.clear();
    ss << sqn;
    ss >> s_sqn; ss.clear();

    if(cmd.at(5) == 0x60){          //если POLL
        s_type = "POLL";
        to_str = "type:" + s_type + ",addr:" + s_addr + ",sqn:" + s_sqn;
    }

    if(cmd.at(5) == 0x6A){          //если BUZZ
        s_type = "BUZ";
        reader = (int)cmd.at(6);
        tone = (int)cmd.at(7);
        on = (int)cmd.at(8);
        off = (int)cmd.at(9);
        count = (int)cmd.at(10);
        ss << reader;
        ss >> s_reader; ss.clear();
        ss << tone ;
        ss >> s_tone; ss.clear();
        ss << on ;
        ss >> s_on; ss.clear();
        ss << off;
        ss >> s_off; ss.clear();
        ss << count;
        ss >> s_count; ss.clear();

        to_str = "type:" + s_type + ",addr:" + s_addr + ",sqn:" + s_sqn + ",reader:" + s_reader + ",tone:" 
            + s_tone + ",on:" + s_on + ",off:" + s_off + ",count:" +s_count;
    }
      
    return to_str;
}
