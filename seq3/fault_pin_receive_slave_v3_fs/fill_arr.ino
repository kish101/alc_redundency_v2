void fill_arr()
{
  switch (redundancy_read_str[0])
  {
    case 'M':
      {
        // mute_press_func();
      }//scope
      break;

    case 'a':
      {
        //...........................
        arr_pos_int = atoi(&redundancy_read_str[1]);
        pin = atoi(&redundancy_read_str[redundancy_read_str.indexOf(',') + 1]);
        //.................................
        alarm_arr_int = arr_pos_int;
        alarm_arr[alarm_arr_int] = pin;
        alarm_arr_int++;
        issues++;
        //.....................................
        print_arr(0);
        print_arr(1);
        //...........................
        buz_state_flag = 1;
      }//scope
      break;

    case 'A':
      {
        //...........................
        for (int i = 0; i < alarm_arr_int; i++)
        {

          if (alarm_arr[i] != 201)//33
          {
            pin = alarm_arr[i];
            //                lamp_state(pin, 1);
            after_ack_arr[after_ack_arr_int] = pin;
            after_ack_arr_int++;
            alarm_arr[i] = 201;//33
          }// if !33

        }//for
        //...........................
        buz_state_flag = 0;
        //.....................................
        //        print_arr(0);
        //        print_arr(1);
        print_arr(2);
      }//scope
      break;

    case 'C':
      {
        //...........................
        pin = atoi(&redundancy_read_str[redundancy_read_str.indexOf(',') + 1]);
        //.................................
        after_ack_arr[pin] = 201;
        solved_issues++;
        //.....................................
        print_arr(0);
        print_arr(1);
      }//scope
      break;

    case 'R':
      {
        reset_all();
        //.....................................
        print_arr(0);
        print_arr(1);
        print_arr(2);
      }//scope
      break;
  }//swicth

}//void
