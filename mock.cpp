void mock_function();

void mock_function()
{
    uint8_t a = 0;
    uint16_t b = __UINT8_MAX__ + 1;

    while(a < b)
    {
        a++;
    }
}