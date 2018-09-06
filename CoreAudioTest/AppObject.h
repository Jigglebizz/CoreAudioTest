#pragma once

class Context;

class AppObject {
protected:
    Context& mContext;
public:
    AppObject( Context& C) : mContext(C) {}
    virtual ~AppObject() {};
};
