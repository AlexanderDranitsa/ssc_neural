#include "p_gen.h"
#include <cmath>

P_GEN::P_GEN(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    request("request"),
    req_write("req_write"),
    done("done")
{
    req_write.initialize(0);
    done.initialize(0);

    SC_METHOD(gen);
    sensitive << clk_i.pos();
}

P_GEN::~P_GEN()
{
}

inline float sygm(float x)
{
    float base = 2.71;
    return (1/(1 + (pow(base, -x))));
}

void P_GEN::gen()
{
    req_write.write(0);
    done.write(0);
    rnd = rand();
    if (request.read()){
        cout<<"GOT REQUEST" << endl;
        for (int i = 0; i < INPUT_LENGTH; i++){
                fig[i] = 0;
            }
            for (int i = 0; i < 3; i++){
                ref[i] = 0;
            }
        if ((rnd % 3) == 0) {
            for (int i = 0; i < INPUT_LENGTH; i++){
                fig[i] = m_round[i];
            }
            ref[2] = 1;
        } else if ((rnd % 3) == 1) {
            for (int i = 0; i < INPUT_LENGTH; i++){
                fig[i] = m_triangle[i];
            }
            ref[1] = 1;
        } else {
            for (int i = 0; i < INPUT_LENGTH; i++){
                fig[i] = m_square[i];
            }
            ref[0]= 1;
        }
        int i = HARASS_LVL;
        while(i){
            int rnd= rand() % INPUT_LENGTH;
            if (fig[rnd] == 0){
                fig[rnd] = 1;
                int flag = 1;
                while(flag){
                    int rnd= rand() % INPUT_LENGTH;
                    if (fig[rnd] == 1){
                        fig[rnd] = 0;
                        flag = 0;
                    }
                }
                i--;
            }
        }
        cout << "REF FROM PGEN " << ref[0] << " "<< ref[1] << " "<< ref[2] << " " << endl;
        cout << rnd << endl;
        counter = 0;
        flag_v = 1;
        flag_r = 1;
    }
    if (flag_v){
        shared_a = INPUT - 1 +counter;
        //shared_d = sygm(fig[counter]);
        shared_d = fig[counter];
        //cout << "ADDR: " << shared_a << " DATA: " << shared_d << endl;
        req_write.write(1);
        counter++;
        if (counter == INPUT_LENGTH + 1){
            flag_v = 0;
            counter = 0;
        }
    } else if (flag_r) {
        shared_a = REFERENCE+counter;
        shared_d = ref[counter];
        //cout << "ADDR: " << shared_a << " DATA: " << shared_d << endl;
        req_write.write(1);
        counter++;
        if (counter == 3){
            flag_r = 0;
            counter = 0;
            done.write(1);
        }
    }
}