#include <sstream>

#include <esp_log.h>
#include <esp_err.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <fp1020a.h>


#include "fp_add_view.hpp"
#include "fp_add_controller.hpp"

#define TAG "fp_add_ctrl"

fp_add_controller::fp_add_controller() : view()
{

}


esp_err_t fp_add_controller::preform_enroll()
{
    auto add_progress = view.get_object("add_progress");
    fp_ack_t ret = FP1020A_ACK_SUCCESS;

    // First round
    do {
        view.set_title_label_text("Recording 1/6");
        ret = fp1020a_add_fp_start(1, FP1020A_USER_LEVEL_1);
        if(ret == FP1020A_ACK_TRY_AGAIN) view.set_title_label_text("Please try again");
        else if(ret == FP1020A_ACK_SUCCESS) break;
        else {
            view.set_title_label_text("Unexpected error");
            fp1020a_remove_all_user(); // TODO: remove this later on, maybe
            vTaskDelay(portMAX_DELAY);
        }
    } while(ret == FP1020A_ACK_TRY_AGAIN);
    lv_bar_set_anim_time(add_progress, 200);
    lv_bar_set_value(add_progress, (int16_t)(100.0 / 6.0), LV_ANIM_ON);


    // Intermediate rounds
    for(uint8_t count = 2; count <= 5; count++) {
        do {
            std::stringstream buf;
            buf << "Recording " << (int)count << "/6";
            view.set_title_label_text(buf.str());


            ret = fp1020a_add_fp_intermediate(1, FP1020A_USER_LEVEL_1);
            if(ret == FP1020A_ACK_TRY_AGAIN) view.set_title_label_text("Please try again");
            else if(ret == FP1020A_ACK_SUCCESS) break;
            else {
                view.set_title_label_text("Unexpected error");
                fp1020a_remove_all_user(); // TODO: remove this later on, maybe
                vTaskDelay(portMAX_DELAY);
            }
        } while(ret == FP1020A_ACK_TRY_AGAIN);
        lv_bar_set_anim_time(add_progress, 200);
        lv_bar_set_value(add_progress, (int16_t)(100.0 / 6.0 * (double)count), LV_ANIM_ON);
    }

    // Last round
    do {
        view.set_title_label_text("Recording 6/6");

        ret = fp1020a_add_fp_end(1, FP1020A_USER_LEVEL_1);
        if(ret == FP1020A_ACK_TRY_AGAIN) view.set_title_label_text("Please try again");
        else if(ret == FP1020A_ACK_SUCCESS) break;
        else {
            view.set_title_label_text("Unexpected error");
            fp1020a_remove_all_user(); // TODO: remove this later on, maybe
            vTaskDelay(portMAX_DELAY);
        }
    } while(ret == FP1020A_ACK_TRY_AGAIN);
    lv_bar_set_anim_time(add_progress, 200);
    lv_bar_set_value(add_progress, 100, LV_ANIM_ON);

    view.set_title_label_text("Successfully recorded!");


    return ESP_OK;
}
