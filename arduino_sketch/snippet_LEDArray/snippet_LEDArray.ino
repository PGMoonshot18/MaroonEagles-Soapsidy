#define STATE_1 2 //ID Accepted
#define STATE_2 3 //ID No Discount
#define STATE_3 4 //ID Yes Discount
#define STATE_4 5 //Soap Ready to Dispense
#define STATE_5 6 //Discount Recieved 
#define STATE_6 7 //Error



void ledArray(int state){
  switch(state){
    case 1:
      digitalWrite(STATE_1, HIGH);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 2:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, HIGH);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 3:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, HIGH);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 4:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, HIGH);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, LOW);
      break;
    case 5:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, HIGH);
      digitalWrite(STATE_6, LOW);
      break;
    case 6:
      digitalWrite(STATE_1, LOW);
      digitalWrite(STATE_2, LOW);
      digitalWrite(STATE_3, LOW);
      digitalWrite(STATE_4, LOW);
      digitalWrite(STATE_5, LOW);
      digitalWrite(STATE_6, HIGH);
      break;
    default:
      digitalWrite(STATE_1, HIGH);
      digitalWrite(STATE_2, HIGH);
      digitalWrite(STATE_3, HIGH);
      digitalWrite(STATE_4, HIGH);
      digitalWrite(STATE_5, HIGH);
      digitalWrite(STATE_6, HIGH);
  }
}

