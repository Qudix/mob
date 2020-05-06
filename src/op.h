#pragma once

#include "utility.h"

namespace mob { class context; }

namespace mob::op
{

enum flags
{
	noflags    = 0x00,
	optional   = 0x01,
	copy_files = 0x02,
	copy_dirs  = 0x04,
	unsafe     = 0x08
};

MOB_ENUM_OPERATORS(flags);


void touch(const context& cx, const fs::path& p);

void create_directories(
	const context& cx, const fs::path& p);

void delete_directory(
	const context& cx, const fs::path& p, flags f=noflags);

void delete_file(
	const context& cx, const fs::path& p, flags f=noflags);

void remove_readonly(
	const context& cx, const fs::path& first);

void rename(
	const context& cx, const fs::path& src, const fs::path& dest);

void move_to_directory(
	const context& cx, const fs::path& src, const fs::path& dest_dir);

void copy_file_to_dir_if_better(
	const context& cx,
	const fs::path& file, const fs::path& dest_dir, flags f=noflags);

void copy_file_to_file_if_better(
	const context& cx,
	const fs::path& src_file, const fs::path& dest_file, flags f=noflags);

void copy_glob_to_dir_if_better(
	const context& cx,
	const fs::path& src_glob, const fs::path& dest_dir, flags f);

std::string read_text_file(
	const context& cx, const fs::path& p, flags f=noflags);

void write_text_file(
	const context& cx, const fs::path& p, std::string_view s, flags f=noflags);

}	// namespace
