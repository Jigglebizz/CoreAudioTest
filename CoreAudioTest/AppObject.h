#pragma once

class Context;

/**
 * Do you need the Context? Ok you're an AppObject
 */
class AppObject {
protected:
    Context& mContext;
public:
    AppObject( Context& C) : mContext(C) {}
    virtual ~AppObject() {};
};
