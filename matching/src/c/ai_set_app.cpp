class AI {
public:
    unsigned int app_bits;
    void set_app(unsigned int app);
};

// FUNCTION: SWKOTOR 0x00604790
void AI::set_app(unsigned int app)
{
    app_bits = app;
}
