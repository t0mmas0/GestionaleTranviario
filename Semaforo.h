/**
 * @author Tommaso Bertoldi 1218661
 * @brief come da nome la classe rappresenta un semaforo della linea ferroviaria
 */

#ifndef GESTIONALETRANVIARIO_SEMAFORO_H
#define GESTIONALETRANVIARIO_SEMAFORO_H


class Semaforo {
private:
    bool status;
public:
    Semaforo();

    void setRosso();
    void setVerde();
    bool getStatus();

};


#endif //GESTIONALETRANVIARIO_SEMAFORO_H
