
#include "relay.h"

uint8_t relay3,relay1,relay2;
volatile uint8_t relay_update_flag;

void relay_control(uint8_t pinmask, uint8_t pinval)
{
  uint8_t newState1, newState2, newState3;
  uint8_t newSate = 0xFF;
  relay_update_flag = (pinmask<<4) | (pinval);
  // Save state
  if (pinmask & RELAY1_ON)
  {
    if (pinval & RELAY1_ON)
    {
      newState1 = RELAY_ON;
    }
    else
    {
      newState1 = RELAY_OFF;

    }
    newSate = newState1;
    if (newState1 != relay1)
    {
      relay1 = newState1;
     EEpromSaveRelay(0,relay1);
    }
  }
//---------------------------
  if (pinmask & RELAY2_ON)
  {

    if (pinval & RELAY2_ON)
    {
      newState2 = RELAY_ON;
    }
    else
    {
      newState2 = RELAY_OFF;
    }
    newSate = newState2;
    if (newState2 != relay2)
    {
      relay2= newState2;
     EEpromSaveRelay(1,relay2);
    }
  }
//------------------------------
  if (pinmask & RELAY3_ON)
  {
    if (pinval & RELAY3_ON)
    {
      newState3 = RELAY_ON;
    }
    else
    {
      newState3 = RELAY_OFF;
    }
    newSate = newState3;
    if (newState3 != relay3)
    {
      relay3= newState3;
     EEpromSaveRelay(2,relay3);
    }
  }
  if (newSate == RELAY_ON)
  {
    TMR_SetComp1Val(CH0,ramcfg.relay_on_delay);
  }
  else if (newSate == RELAY_OFF)
  {
    TMR_SetComp1Val(CH0,ramcfg.relay_off_delay);
  }
}


/* TMR0 Callback */
void tmr0Callback(TMR_CH_CALLBACK_TYPE type, int32 result)
{
  uint8_t pinmask=0;
  uint8_t pinval=0;
  if (type == COMPARE_CALLBACK) 
  {
    if (relay_update_flag)
    {
      pinmask = relay_update_flag >> 4;
      pinval  = relay_update_flag & 0x00FF;
      relay_update_flag = 0;
      if (pinmask & RELAY1_ON)
      {
        if (pinval & RELAY1_ON)
        {
          RELAY1_PDOR |= RELAY1_PIN;
        }
        else
        {
          RELAY1_PDOR &= ~RELAY1_PIN;
        }
      }
      if (pinmask & RELAY2_ON)
      {
        if (pinval & RELAY2_ON)
        {
          RELAY2_PDOR |= RELAY2_PIN;
        }
        else
        {
          RELAY2_PDOR &= ~RELAY2_PIN;
        }
      }
      if (pinmask & RELAY3_ON)
      {
        if (pinval & RELAY3_ON)
        {
          RELAY3_PDOR |= RELAY3_PIN;
        }
        else
        {
          RELAY3_PDOR &= ~RELAY3_PIN;
        }
      }
    }
    TMR_SetComp1Val(CH0,1);
  }
}

