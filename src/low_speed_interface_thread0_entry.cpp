#include <Data_structs/Can-Header-Map/CANOPEN_db.h>
#include <Data_structs/Store.cpp>

#include <string.h>
#include <stdlib.h>
#include <functional>
#include <low_speed_interface_thread0.h>

#include "Interfaces/HighSpeedAbsL.cpp"
#include "Interfaces/Drivers/HardwareBased/CanFDRen.h"
#include "utils.h"

#include "Data_structs/Can-Header-Map/CAN_asdb.h"



void * interface_callback0_t;
void * interface_callback1_t;
/* CANFD Channel 1 Acceptance Filter List (AFL) rule array */
extern "C" const canfd_afl_entry_t p_canfd0_afl[CANFD_CFG_AFL_CH0_RULE_NUM] ={
      {
          .id =
            {
              .id = 0x1FFFFFFF,
              .frame_type = CAN_FRAME_TYPE_DATA,
              .id_mode    = CAN_ID_MODE_EXTENDED,

            },
          .mask =
            {
              .mask_id         = 0,
              .mask_frame_type = 1,
              .mask_id_mode    = 1,
            },
          .destination =
            {
              .minimum_dlc = CANFD_MINIMUM_DLC_0,
              .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_0,
              .fifo_select_flags = CANFD_RX_FIFO_0,
            }
      },{
          .id =
            {
              .id = 0x1FFFFFFF,
              .frame_type = CAN_FRAME_TYPE_DATA,
              .id_mode    = CAN_ID_MODE_STANDARD,
            },
          .mask =
            {
              .mask_id         = 0,
              .mask_frame_type = 1,
              .mask_id_mode    = 1,
            },
          .destination =
            {
              .minimum_dlc = CANFD_MINIMUM_DLC_0,
              .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_1,
              .fifo_select_flags = CANFD_RX_FIFO_1,
            }
        }

};
extern "C" const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM] ={
      {
               .id =
                 {
                   .id = 0x1FFFFFFF,
                   .frame_type = CAN_FRAME_TYPE_DATA,
                   .id_mode    = CAN_ID_MODE_STANDARD,

                 },
               .mask =
                 {
                   .mask_id         = 0,
                   .mask_frame_type = 1,
                   .mask_id_mode    = 1,
                 },
               .destination =
                 {
                   .minimum_dlc = CANFD_MINIMUM_DLC_0,
                   .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_0,
                   .fifo_select_flags = CANFD_RX_FIFO_0,
                 }
           },{
               .id =
                 {
                   .id = 0x1FFFFFFF,
                   .frame_type = CAN_FRAME_TYPE_DATA,
                   .id_mode    = CAN_ID_MODE_STANDARD,
                 },
               .mask =
                 {
                   .mask_id         = 0,
                   .mask_frame_type = 1,
                   .mask_id_mode    = 1,
                 },
               .destination =
                 {
                   .minimum_dlc = CANFD_MINIMUM_DLC_0,
                   .rx_buffer   = (canfd_rx_mb_t) CANFD_RX_BUFFER_FIFO_1,
                   .fifo_select_flags = CANFD_RX_FIFO_1,
                 }
             }

};

extern "C" void canfd0_callback(can_callback_args_t * p_args);
extern "C" void canfd1_callback(can_callback_args_t * p_args);
UINT wakeupNodes(std::shared_ptr<CanFDRen> canfd);
bool containsAllNodes(std::unordered_set<int> source, std::unordered_set<int> target);

void low_speed_interface_thread0_entry(void) {
    can_frame_t frame;
    HighSpeed_AbsL<CanFDRen> canfd0;
    HighSpeed_AbsL<CanFDRen> canfd1;
    interface_callback0_t=(void *)&canfd0;
    interface_callback1_t=(void *)&canfd1;
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_SECONDS);
    wakeupNodes(canfd1.g_AplHandle());


    while(1){

        R_BSP_SoftwareDelay(4000, BSP_DELAY_UNITS_MILLISECONDS);

        //canfd1->decodeImmediate (frame);


    }





    //frame.options = CANFD_FRAME_OPTION_BRS | CANFD_FRAME_OPTION_FD;
    /*critical_as temp_data;
     *
      tx_semaphore_get(&css, 32);
          temp_data = store::critical_autonomous_system_status;
      tx_semaphore_put(&css);*/





    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    while(1){

        R_BSP_SoftwareDelay(500, BSP_DELAY_UNITS_MILLISECONDS);

    }


}
UINT drive(std::shared_ptr<CanFDRen> canfd){
    return FSP_SUCCESS;
}
UINT wakeupNodes(std::shared_ptr<CanFDRen> canfd){
        can_frame_t frame;
        can_frame_stream _data;
        frame.id = NMT_ADDRESS_COBID();
        frame.data_length_code = 8U;
        frame.id_mode  = CAN_ID_MODE_STANDARD;
        frame.type = CAN_FRAME_TYPE_DATA;
        frame.options = 0;
        canfd->preamble((void *)&frame);
        //TODO for speed we can afford to pop the inscribed_nodes_list uppon one member booting
        while(containsAllNodes(canfd->currentCanOpenStack->g_bootedNodes(), canfd->currentCanOpenStack->inscribed_nodes_list) == false){
            _data = canfd->currentCanOpenStack->nmt_message(NMT_ENTER_PRE_OPERATIONAL, 0);
            canfd->write((void *)&_data,8, true);
            R_BSP_SoftwareDelay(75, BSP_DELAY_UNITS_MILLISECONDS);
        }

        _data = canfd->currentCanOpenStack->nmt_message(NMT_START_REMOTE_NODE, 0);
        canfd->write((void *)&_data,8, true);
        return FSP_SUCCESS;
}
UINT rundownProtocol(std::shared_ptr<CanFDRen> canfd){
    can_frame_t frame;
    can_frame_stream _data;
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    frame.id = SDO_REQUEST_ADDRESS_COBID();
    _data = canfd->currentCanOpenStack->requestStatusWordMessage();
    memcpy(frame.data, &_data.data, 8);
    //bind the response of statusword to the callback function
    canfd->currentCanOpenStack->callback = std::bind(&CANopenStack::readStatusWordMessage, canfd->currentCanOpenStack, std::placeholders::_1);
    canfd->write((void *)&frame,0);
    volatile StateMachine_StatusWord status=STATUSWORD_UNKNOWN;
    do{
       status = canfd->currentCanOpenStack->g_currentState();
       R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    }while(status != STATUSWORD_UNKNOWN);
    R_BSP_SoftwareDelay(50, BSP_DELAY_UNITS_MILLISECONDS);
      //Activate it

        switch(status){
          case Switch_on_disabled:
              _data = canfd->currentCanOpenStack->requestControlWordMessage(0x00, dcc_shutdown);
              memcpy(frame.data, &_data.data, 8);
              canfd->write((void *)&frame,0);
              R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
              [[fallthrough]];
          case Ready_to_switch_on:
              _data = canfd->currentCanOpenStack->requestControlWordMessage(0x00, dcc_switchon_and_enable);
              memcpy(frame.data, &_data.data, 8);
              canfd->write((void *)&frame,0);
              R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
              break;
          case Operation_enabled:
              led_update(3, BSP_IO_LEVEL_HIGH);
              break;

          case Quick_stop_active:
          case Fault_reaction_active:
          case STATUSWORD_UNKNOWN:
          default:
              led_update(0, BSP_IO_LEVEL_HIGH);




      }
      uint8_t __data[8] = {0x22, 0x7A, 0xAA, 0x82, 0x07, 00, 00, 00};
      memcpy(frame.data, __data, 8);


      frame.id = PDO_RXTHREE(NODE_ID_STEERING) ;
      canfd->write((void *)&frame,0);
      R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
      uint8_t data___[8] = {0x22, 0x40, 0xAA, 0x82, 0x07, 0x00, 0x00 ,0x00};
      memcpy(frame.data, data___, 8);
      frame.id = PDO_RXTHREE(NODE_ID_STEERING);
      canfd->write((void *)&frame,0);
      R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
      //write control word
//            frame.id = SDO_REQUEST_ADDRESS_COBID();
//            can_frame_stream _data2 = canfd1->currentCanOpenStack->requestControlWordMessage(0x00, 0x0F);
//            memcpy(frame.data, &_data2.data, 8);
//            canfd1->write((void *)&frame,0);
      frame.id = SDO_REQUEST_ADDRESS_COBID();
      _data = canfd->currentCanOpenStack->requestControlWordMessage(0x00, 0x1F);
      memcpy(frame.data, &_data.data, 8);
      canfd->write((void *)&frame,0);
      R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

      return FSP_SUCCESS;
}

/* Callback function */
extern "C" void canfd0_callback(can_callback_args_t *p_args){

  if(interface_callback0_t != NULL){
      ((CanFDRen *)interface_callback0_t)->callbackHandle(p_args);
  }
}

extern "C" void canfd1_callback(can_callback_args_t *p_args){
  if(interface_callback1_t != NULL){
      ((CanFDRen *)interface_callback1_t)->callbackHandle(p_args);
  }
}
bool containsAllNodes(std::unordered_set<int> source, std::unordered_set<int> target){
    for (auto &i : source){
        if (target.find (i) == target.end ()){
            return false;
        }
    }
    return true;
}
