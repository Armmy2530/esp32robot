int error = 0, pre_error = 0, sum_error;

bool W(int n)
{
  return n == 1;
}
bool B(int n)
{
  return n == 0;
}

void trackline_pid(float pid_parameter[3],int base_speed)
{
    readSensor();
    if (W(sensor_bool[0]) && W(sensor_bool[1]) && W(sensor_bool[2]) && W(sensor_bool[3]) && B(sensor_bool[4]))
    {
        error = 4; // w w w w b
    }
    else if (W(sensor_bool[0]) && W(sensor_bool[1]) && W(sensor_bool[2]) && B(sensor_bool[3]) && B(sensor_bool[4]))
    {
        error = 3; // w w w b b
    }
    else if (W(sensor_bool[0]) && W(sensor_bool[1]) && W(sensor_bool[2]) && B(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = 2; // w w w b w
    }
    else if (W(sensor_bool[0]) && W(sensor_bool[1]) && B(sensor_bool[2]) && B(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = 1; // w w b b w
    }
    else if (W(sensor_bool[0]) && W(sensor_bool[1]) && B(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = 0; // w w b w w
    }
    else if (W(sensor_bool[0]) && B(sensor_bool[1]) && B(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = -1; // w b b w w
    }
    else if (W(sensor_bool[0]) && B(sensor_bool[1]) && W(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = -2; // w b w w w
    }
    else if (B(sensor_bool[0]) && B(sensor_bool[1]) && W(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = -3; // b b w w w
    }
    else if (B(sensor_bool[0]) && W(sensor_bool[1]) && W(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = -4; // b w w w w
    }
    else if (W(sensor_bool[0]) && W(sensor_bool[1]) && W(sensor_bool[2]) && W(sensor_bool[3]) && W(sensor_bool[4]))
    {
        error = pre_error; // w w w w w
    }
    int motorSpeed = (int)(pid_parameter[0] * error +pid_parameter[1] * (error - pre_error) + pid_parameter[2] * (sum_error));
    int leftSpeed = base_speed + motorSpeed;
    int rightSpeed = base_speed - motorSpeed;

    m(leftSpeed, rightSpeed);
    pre_error = error;
    sum_error += error;
}
void trackline_R(float pid[3],int base_speed,int line,int finalmove_speed,int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid,base_speed);
        if (B(sensor_bool[4]) && B(sensor_bool[3]))
        {
            delay(100);
            if (B(sensor_bool[4]) && B(sensor_bool[3]))
            {
                while (B(sensor_bool[4]) && B(sensor_bool[3]))
                {
                    fd(finalmove_speed, finalmove_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    delay(ms_delay);
    stop(false);
}

void trackline_L(float pid[3],int base_speed,int line,int finalmove_speed,int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid,base_speed);
        if (B(sensor_bool[0]) && B(sensor_bool[1]))
        {
            delay(100);
            if (B(sensor_bool[0]) && B(sensor_bool[1]))
            {
                while (B(sensor_bool[0]) && B(sensor_bool[1]))
                {
                    fd(finalmove_speed, finalmove_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    delay(ms_delay);
    stop(false);
}

void trackline_Cross(float pid[3],int base_speed,int line,int finalmove_speed,int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid,base_speed);
        if (B(sensor_bool[4]) && B(sensor_bool[0]))
        {
            delay(100);
            if (sensor_bool[4] == 0 && sensor_bool[0] == 0)
            {
                while (sensor_bool[0] == 0 && sensor_bool[4] == 0)
                {
                    fd(finalmove_speed, finalmove_speed);
                    readSensor();
                }
                count++;
            }
        }
    }
    delay(ms_delay);
    stop(false);
}

void tr_sensor(int L_speed, int R_speed)
{
    fd(L_speed, R_speed);
    delay(100);
    stop(false);
    delay(100);
    sr(L_speed, R_speed);
    delay(400);
    stop(false);
    readSensor();
    while (sensor_bool[2] == 1)
    {
        sr(L_speed, R_speed);
        readSensor();
    }
    stop(false);
}


void tl_sensor(int L_speed, int R_speed)
{
    fd(L_speed, R_speed);
    delay(100);
    stop(false);
    delay(100);
    sl(L_speed, R_speed);
    delay(400);
    stop(false);
    readSensor();
    while (sensor_bool[2] == 1)
    {
        sl(L_speed, R_speed);
        readSensor();
    }
    stop(false);
}
