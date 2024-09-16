#include "Logger.hpp"

#include "Setup.hpp"

#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <filesystem>
#include <cassert>


namespace game
{
Logger::Logger() noexcept
{
  ZoneScopedC(0xbaed00);

  std::filesystem::create_directories(kDefLogPath);
  file_.open(file_path_, std::ios_base::out | std::ios_base::trunc);
  GAME_ASSERT(file_.good()) << "Couldn't creaate file: " << file_path_ << " when initialized LogStream: " << this;
}

Logger::~Logger() noexcept
{
  ZoneScopedC(0xbaed00);

  file_.close();
}

LogStream::~LogStream() noexcept
{
  ZoneScopedNC("Log whole", 0xbaed00);

  string_stream << '\n';
  Logger::Get().GetStream() << string_stream.str();
  if(Logger::Get().IsOutputToFile())
  {
	  Logger::Get().GetFile() << string_stream.str();
	  Logger::Get().GetFile() .flush();
  }

	string_stream.str("");

  if(fatal_)
    std::abort();
}
} // game