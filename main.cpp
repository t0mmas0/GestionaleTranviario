#include <iostream>
#include "LineaFerroviaria.h"
#include "Grafica.h"
int main() {
    std::cout<<"            ___________   _______________________________________^__\n";
    std::cout<<"             ___   ___ |||  ___   ___   ___    ___ ___  |   __  ,----\\ \n";
    std::cout<<"            |   | |   |||| |   | |   | |   |  |   |   | |  |  | |_____\\ \n";
    std::cout<<"            |___| |___|||| |___| |___| |___|  | O | O | |  |  |        \\ \n";
    std::cout<<"                       |||                    |___|___| |  |__|         ) \n";
    std::cout<<"            ___________|||______________________________|______________/ \n";
    std::cout<<"                       |||                                        /-------- \n";
    std::cout<<"            -----------'''---------------------------------------' \n";
    std::cout<<std::endl;
    std::cout<<"                                  _             _        \n";
    std::cout<<"                                 | |_ _ __ __ _(_)_ __  ___ \n";
    std::cout<<"                                 | __| '__/ _` | | '_ \\/ __|\n";
    std::cout<<"                                 | |_| | | (_| | | | | \\__ \\\n";
    std::cout<<"                                  \\__|_|  \\__,_|_|_| |_|___/\n";
    std::cout<<"                                       ____                                                     \n";
    std::cout<<"                                      |  _ \\                                                    \n";
    std::cout<<"                                      | |_) |_   _                                              \n";
    std::cout<<"                                      |  _ <| | | |                                             \n";
    std::cout<<"                                      | |_) | |_| |                                             \n";
    std::cout<<"            _______                   |____/ \\__, |          ____            _        _     _ _ \n";
    std::cout<<"           |__   __|                          __/ |         |  _ \\          | |      | |   | (_)\n";
    std::cout<<"              | | ___  _ __ ___  _ __ ___   _|___/__  ___   | |_) | ___ _ __| |_ ___ | | __| |_ \n";
    std::cout<<"              | |/ _ \\| '_ ` _ \\| '_ ` _ \\ / _` / __|/ _ \\  |  _ < / _ \\ '__| __/ _ \\| |/ _` | |\n";
    std::cout<<"              | | (_) | | | | | | | | | | | (_| \\__ \\ (_) | | |_) |  __/ |  | || (_) | | (_| | |\n";
    std::cout<<"            __|_|\\___/|_| |_| |_|_| |_| |_|\\__,_|___/\\___/  |____/ \\___|_|   \\__\\___/|_|\\__,_|_|\n";
    std::cout<<"            |  \\/  |                     | \\ | |             | (_)                               \n";
    std::cout<<"            | \\  / | __ _ _ __ ___ ___   |  \\| | __ _ _ __ __| |_                                \n";
    std::cout<<"            | |\\/| |/ _` | '__/ __/ _ \\  | . ` |/ _` | '__/ _` | |                               \n";
    std::cout<<"            | |  | | (_| | | | (_| (_) | | |\\  | (_| | | | (_| | |                               \n";
    std::cout<<"            |_|  |_|\\__,_|_|  \\___\\___/  |_| \\_|\\__,_|_|  \\__,_|_|                               \n";
    std::cout<<"              _____ _                              _____                                         \n";
    std::cout<<"             / ____(_)                            |  __ \\                                        \n";
    std::cout<<"            | (___  _ _ __ ___   ___  _ __   ___  | |__) |__ _ __ __ _ _ __ ___                  \n";
    std::cout<<"             \\___ \\| | '_ ` _ \\ / _ \\| '_ \\ / _ \\ |  ___/ _ \\ '__/ _` | '__/ _ \\                 \n";
    std::cout<<"             ____) | | | | | | | (_) | | | |  __/ | |  |  __/ | | (_| | | | (_) |                \n";
    std::cout<<"            |_____/|_|_| |_| |_|\\___/|_| |_|\\___| |_|   \\___|_|  \\__,_|_|  \\___/                 " << std::endl;
    std::cout<<"\n";
    std::cout<<"                                                 (@@@)     (@@@@@)\n";
    std::cout<<"                                           (@@)     (@@@@@@@)        (@@@@@@@)\n";
    std::cout<<"                                     (@@@@@@@)   (@@@@@)       (@@@@@@@@@@@)\n";
    std::cout<<"                                (@@@)     (@@@@@@@)   (@@@@@@)             (@@@)\n";
    std::cout<<"                           (@@@@@@)    (@@@@@@)                (@)\n";
    std::cout<<"                       (@@@)  (@@@@)           (@@)\n";
    std::cout<<"                    (@@)              (@@@)\n";
    std::cout<<"                   .-.               \n";
    std::cout<<"                   ] [    .-.      _    .-----.\n";
    std::cout<<"                 .\"   \"\"\"\"   \"\"\"\"\"\" \"\"\"\"| .--`\n";
    std::cout<<"                (:--:--:--:--:--:--:--:-| [___    .------------------------.\n";
    std::cout<<"                 |     :  :  :  :  :  : [_9_] |'='|.----------------------.|\n";
    std::cout<<"                /|.___________________________|___|'--.___.--.___.--.___.-'| \n";
    std::cout<<"               / ||_.--.______.--.______.--._ |---\\'--\\-.-/==\\-.-/==\\-.-/-'/--\n";
    std::cout<<"              /__;^=(==)======(==)======(==)=^~^^^ ^^^^(-)^^^^(-)^^^^(-)^^^    \n";
    std::cout<<"            ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;


    LineaFerroviaria linea{ "../../../timetables.txt", "../../../line_description1.txt" };
    linea.esegui();
}