void fill_arr()
{
  switch (read_str[0])
  {
    case 'a':
      {
        //...........................
        arr_pos_int = atoi(&read_str[1]);
        pin = atoi(&read_str[read_str.indexOf(',') + 1]);
        //.................................
        alarm_arr_int = arr_pos_int;
        alarm_arr[alarm_arr_int] = pin;
        alarm_arr_int++;
        issues++;
        //.....................................
        print_arr(0);
        print_arr(1);
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
        //.....................................
        //        print_arr(0);
        //        print_arr(1);
        print_arr(2);
      }//scope
      break;

    case 'C':
      {
        //...........................
        pin = atoi(&read_str[read_str.indexOf(',') + 1]);
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
