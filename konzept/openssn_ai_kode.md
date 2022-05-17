# Bot-Logik von OpenSSN

Lizenz-Header

```cpp
/***************************************************************************
                          submarine.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Michael Bridak
    email                : michael.bridak@verizon.net
$Id: submarine.cpp,v 1.6 2003/04/14 05:51:04 mbridak Exp $
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.     *
 *    .                                   *
 *                                                                         *
 ***************************************************************************/
```

Bot-Kode
```cpp

// This function tells us what AI ships and submarines will do.
// This function returns a link to all torpedoes.
Submarine *Submarine::Sub_AI(Submarine *all_ships, Submarine *all_torpedoes, void *helicopters)
{
   int change;
   Submarine *torpedo;
   int can_hear_torpedo;
   double distance;
   int bearing;
   Submarine *target = NULL, *my_torpedoes;
   int status, found;
   int action = 0;     // 1 = running, 2 = chasing
   Submarine *track_torpedo = NULL;

   // most important thing we can do is run away from torpedoes
   if (has_sonar)
   {
       #ifdef AIDEBUG
       printf("We have sonar.\n");
       #endif
       // go through all torpedoes and see if any of them
       // chasing us
       torpedo = all_torpedoes;
       status = FALSE;   // make sure we only run away form one torpedo
       while ( (torpedo) && (! status) )
       {
           if (torpedo->ShipType == TYPE_TORPEDO) 
           {
           can_hear_torpedo = Can_Hear(torpedo);
           distance = DistanceToTarget(torpedo);
           // if we can hear a torpedo, it is close and it is aimed at us
           // then we run
           if ( (can_hear_torpedo) && (torpedo->target == this) &&
                (distance < (MAX_TORPEDO_RANGE * MILES_TO_YARDS) ) )
           {
               // status = (DesiredSpeed == MaxSpeed);
               all_torpedoes = Launch_Noisemaker(all_torpedoes, torpedo);
               bearing = (int) BearingToTarget(torpedo);
               bearing += 180;
               if (bearing >= 360)
                  bearing = bearing % 360;
               DesiredHeading = bearing;
               DesiredSpeed = MaxSpeed;
               // subs can dive too
               if (ShipType == TYPE_SUB)
               {
                 if (torpedo->Depth <= Depth)  // it is above us
                     DesiredDepth = MaxDepth;
                 else if (torpedo->Depth > Depth)  // below us
                     DesiredDepth = PERISCOPE_DEPTH;
               }
               if (mood == MOOD_CONVOY)
                  mood = MOOD_PASSIVE;
               // if (! status)
               //   return all_torpedoes;
               action = 1;  // running
               status = TRUE;
           }
           // we hear a torpedo but it is not coming after us
           else if ( (can_hear_torpedo) && (TorpedosOnBoard) )
           {
                if (ShipType == TYPE_SHIP)
                    mood = MOOD_ATTACK;
                // if we are not tracking anything, check out the torp
                if ( (! track_torpedo) && (torpedo->Friend != Friend) )
                {
                  #ifdef AIDEBUG
                  printf("Found torpedo to track.\n");
                  #endif
                  track_torpedo = torpedo;
                }
           }
          
           if ( (can_hear_torpedo) && (ShipType == TYPE_SHIP) )
              Radio_Signal(all_ships, RADIO_HEAR_TORPEDO);

           }   // end of this is a torpedo
           torpedo = torpedo->next;
        }   // end of checking out torpedoes in the water

      #ifdef AIDEBUG
      printf("Checking for targets.\n");
      #endif
      // see if we can hear a nearby enemy to shoot at
      target = Have_Enemy_Target(all_ships);
      #ifdef AIDEBUG
      if (target) printf("Found enemy.\n");
      #endif
      int count = Count_Torpedoes(all_torpedoes);
      if ( (target) && (TorpedosOnBoard > 0) ) 
           // (count < MAX_TORPEDOES_FIRED) )
      {
          int target_range = DistanceToTarget(target);
          int shooting_range;
          // we shoot at different distances, depending on mood
          if (mood == MOOD_ATTACK)
            shooting_range = TORPEDO_RANGE_ATTACK;
          else
            shooting_range = TORPEDO_RANGE_PASSIVE;
          shooting_range += (rand() % 3) - 2;  // add some randomness
          target_range *= YARDS_TO_MILES;
          #ifdef AIDEBUG
          printf("Checking range %d\n", target_range);
          #endif
          if ( (count < MAX_TORPEDOES_FIRED) && 
               (target_range < shooting_range) )
          {
          #ifdef AIDEBUG
          printf("Firing with %d torpedoes.\n", count);
          #endif
          torpedo_tube[0] = TUBE_TORPEDO;
          status = Use_Tube(FIRE_TUBE, 0);
          if (status == TUBE_ERROR_FIRE_SUCCESS)
          {
              char *ship_file, filename[] = "ships/class5.shp";
              ship_file = Find_Data_File(filename);
              torpedo = Fire_Tube(target, ship_file );
              if ( (ship_file) && (ship_file != filename) )
                   free(ship_file);
              if (torpedo)
              {
                 #ifdef AIDEBUG
                 printf("Successfully fired torpedo.\n");
                 #endif
                 torpedo->Friend = Friend;
                 torpedo->owner = this;
                 // all_torpedoes = Add_Ship(all_torpedoes, torpedo);
                 // add torpedo to linked list
                 if (! all_torpedoes)
                    all_torpedoes = torpedo;
                 else
                 {
                   my_torpedoes = all_torpedoes;
                   found = FALSE;
                   while ( (! found) && (my_torpedoes) )
                   {
                       if (my_torpedoes->next)
                          my_torpedoes = my_torpedoes->next;
                       else
                       {
                         my_torpedoes->next = torpedo;
                         found = TRUE;
                         // return all_torpedoes;
                       }
                   }
                  }  // add torpedo to list 
              }
              TorpedosOnBoard--;
              #ifdef AIDEBUG
              printf("I have %d torpedoes left.\n", TorpedosOnBoard);
              #endif
              return all_torpedoes;
          }   // torpedo firing was successful
          }   // can fire torpedo at target in range
        
          // we hear an enemy, but can't fire yet
          else if ( (mood == MOOD_ATTACK) && (! action) ) 
          {
             DesiredHeading = BearingToTarget(target); 
             if (ShipType == TYPE_SHIP)
                DesiredSpeed = (MaxSpeed / 2) + (rand() % 5) - 2;
             else  // sub
                DesiredSpeed = (MaxSpeed / 3) + (rand() % 3) + 1;
             if (ShipType == TYPE_SUB)
             {
                DesiredDepth = target->Depth;
                if (DesiredDepth < PERISCOPE_DEPTH)
                    DesiredDepth = PERISCOPE_DEPTH;
             }
             action = 2;
          }
      }    // end of we have a target and we have torpedoes on board

      // we are not tracking anything, but we have torpedoes
      // and we can hear a torpedo
      // move toward the origin of the torpedo we hear
      else if ( (TorpedosOnBoard > 0) && (track_torpedo) && (!action) )
      {
         #ifdef AIDEBUG
         printf("We hear a torpedo, tracking it.\n");
         printf("We should probably only chase torpedoes if in attack mood\n");
         #endif
         DesiredHeading = BearingToOrigin(track_torpedo);
         if (ShipType == TYPE_SUB)
            DesiredDepth = track_torpedo->Depth;
      }
      // We have no target and no torpedo to follow
      // if we are in attack mood we should slow down
      // so we can hear better
      else if ( (! action) && (!target) && (mood == MOOD_ATTACK) )
      {
          DesiredSpeed = (MaxSpeed / 3) + ( ( rand() % 5 ) - 3 );
      }
      

      // if we got this far we cannot hear a torpedo coming at us
      if ( (Speed == MaxSpeed) && (! action) )
         DesiredSpeed = MaxSpeed / 3;
   }     // I think this is the end of "we have sonar" section

   // helicopters should go low if there are helicopters
   if ( (ShipType == TYPE_SUB) && (helicopters) )
   {
       if (Depth < map->thermals[0])
          DesiredDepth = map->thermals[0] + 25;
   }
   
   // when traveling in convoy, we change course once every
   // ... twenty minutes?
   if (mood == MOOD_CONVOY)
   {
      convoy_course_change--;
      if (! convoy_course_change)
      {
          convoy_course_change = CONVOY_CHANGE_COURSE;
          DesiredHeading += 90;
          if (DesiredHeading >= 360)
              DesiredHeading = DesiredHeading % 360;
      }
   }
   else   // got nothing to do, but perhaps change course
   {
     change = rand() % CHANCE_COURSE;
     if (! change)
     {
       DesiredHeading = Heading + ( (rand() % 100) - 90);
       if (DesiredHeading >= 360)
         DesiredHeading = DesiredHeading % 360;
       DesiredSpeed = MaxSpeed / 3;
       // submarines should change depth too
       if (ShipType == TYPE_SUB)
       {
          DesiredDepth =+ rand() % 200;
          DesiredDepth /= 2;
          DesiredDepth += PERISCOPE_DEPTH;
          // DesiredDepth = ( (rand() % MaxDepth) / 2 ) + 50;
       }
     }
   }

   // check for radio message
   if ( (ShipType == TYPE_SHIP) && (radio_message) )
   {
       if ( (radio_message == RADIO_UNDER_ATTACK) && (mood == MOOD_CONVOY) )
       {
           if (TorpedosOnBoard)
             mood = MOOD_ATTACK;
           else
           {
             mood = MOOD_PASSIVE;
             DesiredHeading = rand() % 360;
           }
           DesiredSpeed = MaxSpeed;
       }
       else if ( (radio_message == RADIO_HEAR_TORPEDO) && (mood == MOOD_CONVOY) )
       {
           if (TorpedosOnBoard)
              mood = MOOD_ATTACK;
           else
              mood = MOOD_PASSIVE;
       }
   }
   radio_message = RADIO_NONE;
   if ( (ShipType == TYPE_SUB) && (Depth < PERISCOPE_DEPTH) )
       DesiredDepth = PERISCOPE_DEPTH;
   return all_torpedoes;
}  // end of Sub_AI
```