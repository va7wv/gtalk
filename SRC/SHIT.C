
          for (testid=0;testid<num_ports;testid++)

            /* timeout loop */

           if ((line_status[testid].connect) && (user_options[testid].time_sec) &&
               ((user_options[testid].priority>TIMEOUT_PRIORITY)||(full_system) ||
                   !line_status[testid].online || line_status[testid].link))
            {


            dif=(int)((long int)((long int)curtime-(long int)line_status[testid].time_online));
              if ((dif>(user_options[testid].time_warning_sec)) && (!user_options[testid].warnings))
                {
                  user_options[testid].warning_prefix='*';
                  line_status[testid].handlelinechanged=1;

                  if (line_status[testid].link)
                    sprintf(s,"|*h1|*f1--> Link on Node #%02d: Timeout in %d seconds",
                          testid,user_options[testid].time_sec -
                             user_options[testid].time_warning_sec);
                  else
                    sprintf(s,"|*h1|*f1--> Node #%02d: Timeout in %d seconds",
                          testid,user_options[testid].time_sec -
                             user_options[testid].time_warning_sec);

                  user_options[testid].time_sec=(((long int)curtime-(long int)line_status[testid].time_online)+(long int)60);
                  aput_into_buffer(server,s,line_status[testid].mainchannel,8,tswitch,testid,10);
             /*   for (putid=0;putid<num_ports;putid++)
                    aput_into_buffer(putid,s,0,0,0,0,0); <<- fix numbers  */
                  user_options[testid].warnings = 1;
                };
              if (dif>((user_options[testid].time_sec)))
               {
                if (line_status[testid].online && !line_status[testid].link)
                  {
                   print_cr_to(testid);
                   print_str_cr_to("--> Your online time has expired.",testid);
                   print_cr_to(testid);
                  }
                log_off(testid);
               }
            };
