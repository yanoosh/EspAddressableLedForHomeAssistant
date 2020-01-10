class BaseEffect {
  public:
    virtual void begin();
    virtual void loop();
    virtual void end();
};