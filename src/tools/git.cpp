#include "pch.h"
#include "tools.h"
#include "../conf.h"

namespace mob
{

git::git(ops o)
	: basic_process_runner("git"), op_(o)
{
}

fs::path git::binary()
{
	return conf::tool_by_name("git");
}

git& git::url(const mob::url& u)
{
	url_ = u;
	return *this;
}

git& git::branch(const std::string& name)
{
	branch_ = name;
	return *this;
}

git& git::output(const fs::path& dir)
{
	where_ = dir;
	return *this;
}

void git::do_run()
{
	if (url_.empty() || where_.empty())
		bail_out("git missing parameters");

	if (conf::redownload() || conf::reextract())
	{
		cx_->trace(context::rebuild, "deleting directory controlled by git");
		op::delete_directory(*cx_, where_, op::optional);
	}


	switch (op_)
	{
		case clone:
		{
			do_clone();
			break;
		}

		case pull:
		{
			do_pull();
			break;
		}

		case clone_or_pull2:
		{
			do_clone_or_pull();
			break;
		}

		default:
		{
			cx_->bail_out(context::generic, "git unknown op {}", op_);
		}
	}
}

void git::do_clone_or_pull()
{
	if (!do_clone())
		do_pull();
}

bool git::do_clone()
{
	const fs::path dot_git = where_ / ".git";
	if (fs::exists(dot_git))
	{
		cx_->trace(context::generic, "not cloning, {} exists", dot_git);
		return false;
	}

	process_ = process()
		.binary(binary())
		.stderr_level(context::level::trace)
		.arg("clone")
		.arg("--recurse-submodules")
		.arg("--depth", "1")
		.arg("--branch", branch_)
		.arg("--quiet", process::log_quiet)
		.arg("-c", "advice.detachedHead=false", process::log_quiet)
		.arg(url_)
		.arg(where_);

	execute_and_join();

	return true;
}

void git::do_pull()
{
	process_ = process()
		.binary(binary())
		.stderr_level(context::level::trace)
		.arg("pull")
		.arg("--recurse-submodules")
		.arg("--quiet", process::log_quiet)
		.arg(url_)
		.arg(branch_)
		.cwd(where_);

	execute_and_join();
}

}	// namespace
