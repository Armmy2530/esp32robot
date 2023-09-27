#define OUT_LINE 8000 //error code for not found line on any sensor
int error = 0, pre_error = 0, sum_error;

bool W(int n)
{
    return n == 0;
}
bool B(int n)
{
    return n == 1;
}

signed int Cal_Error()
{
    if (sensor_bool[0] || sensor_bool[1] || sensor_bool[2] || sensor_bool[3] || sensor_bool[4])
    {
        // LEFT
        error = (sensor_bool[0]) ? -4 : error;
        error = (sensor_bool[1] && sensor_bool[0]) ? -3 : error;
        error = (sensor_bool[1]) ? -2 : error;
        error = (sensor_bool[2] && sensor_bool[1]) ? -1 : error;

        // CENTER
        error = (sensor_bool[2]) ? 0 : error;

        // RIGHT
        error = (sensor_bool[2] && sensor_bool[3]) ? 1 : error;
        error = (sensor_bool[3]) ? 2 : error;
        error = (sensor_bool[3] && sensor_bool[4]) ? 3 : error;
        error = (sensor_bool[4]) ? 4 : error;

        lastline_state = ((error >= 1) && (error <= 4)) ? RIGHT : lastline_state;
        lastline_state = ((error >= -1) && (error <= 1)) ? CENTER : lastline_state;
        lastline_state = ((error >= -4) && (error <= -2)) ? LEFT : lastline_state;

        return error;
    }
    else
    {
        return OUT_LINE;
    }
}
void trackline_pid(float pid_parameter[3], int base_speed)
{
    // local variable for motor speed
    int leftSpeed,rightSpeed;

    // Read Sensor from analogRead
    readSensor();
    signed int error_actual = Cal_Error();

    // If not find any line spin to lastest line
    if (error_actual == OUT_LINE)
    {
        switch (lastline_state)
        {
        case CENTER:
            leftSpeed = baseSpeed;
            rightSpeed = baseSpeed;
            break;
        case LEFT:
            leftSpeed = -baseSpeed;
            rightSpeed = baseSpeed;
            break;
        case RIGHT:
            leftSpeed = baseSpeed;
            rightSpeed = -baseSpeed;
            break;
        }
    }
    else // if not, then trackline using pid algorithem
    {
        int motorSpeed = (int)(pid_parameter[0] * error + pid_parameter[1] * (error - pre_error) + pid_parameter[2] * (sum_error));
        leftSpeed = base_speed + motorSpeed;
        rightSpeed = base_speed - motorSpeed;
        pre_error = error;
        sum_error += error;
    }
    m(leftSpeed, rightSpeed);
}
void trackline_R(float pid[3], int base_speed, int line, int finalmove_speed, int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid, base_speed);
        if (B(sensor_bool[4]) && B(sensor_bool[3]) && B(sensor_bool[2]))
        {
            readSensor();
            while (B(sensor_bool[4]) && B(sensor_bool[3]) && B(sensor_bool[2]))
            {
                readSensor();
                fd(finalmove_speed);
            }
            count++;
        }
    }
    delay(ms_delay);
    stop(false);
}

void trackline_L(float pid[3], int base_speed, int line, int finalmove_speed, int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid, base_speed);
        if (B(sensor_bool[0]) && B(sensor_bool[1]) && B(sensor_bool[2]))
        {
            while (B(sensor_bool[0]) && B(sensor_bool[1]) && B(sensor_bool[2]))
            {
                fd(finalmove_speed);
                readSensor();
            }
            count++;
        }
    }
    delay(ms_delay);
    stop(false);
}

void trackline_Cross(float pid[3], int base_speed, int line, int finalmove_speed, int ms_delay)
{
    int count = 0;
    while (count < line)
    {
        trackline_pid(pid, base_speed);
        if (B(sensor_bool[4]) && B(sensor_bool[0]))
        {
            while (sensor_bool[0] == 0 && sensor_bool[4] == 0)
            {
                fd(finalmove_speed);
                readSensor();
            }
            count++;
        }
    }
    delay(ms_delay);
    stop(false);
}

void tr_sensor(int speed)
{
    sr(speed);
    delay(200);
    readSensor();
    while (W(sensor_bool[2]))
    {
        sr(speed);
        readSensor();
    }
    stop(false);
}

void tl_sensor(int speed)
{
    sl(speed);
    delay(200);
    readSensor();
    while (W(sensor_bool[2]))
    {
        sl(speed);
        readSensor();
    }
    stop(false);
}
