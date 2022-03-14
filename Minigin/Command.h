#pragma once
class Command 
{
public:
	virtual ~Command() = default;
	virtual void Execute() = 0;
};

class FireCommand : public Command
{
public:
	void Execute() override;
};

class DuckCommand : public Command
{
public:
	void Execute() override;
};

class JumpCommand : public Command
{
public:
	void Execute() override;
};

class FartCommand : public Command
{
public:
	void Execute() override;
};


