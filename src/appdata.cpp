//====================================================================================
//			The MIT License (MIT)
//
//			Copyright (c) 2011 Kapparock LLC
//
//			Permission is hereby granted, free of charge, to any person obtaining a copy
//			of this software and associated documentation files (the "Software"), to deal
//			in the Software without restriction, including without limitation the rights
//			to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//			copies of the Software, and to permit persons to whom the Software is
//			furnished to do so, subject to the following conditions:
//
//			The above copyright notice and this permission notice shall be included in
//			all copies or substantial portions of the Software.
//
//			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//			IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//			FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//			AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//			LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//			OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//			THE SOFTWARE.
//====================================================================================
#include "appdata.h"
#include "notification.h"
#include "apsdb.h"
#include "kjson.h"
#include "restful.h"
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <initializer_list>
#include "hal.h"

namespace
{
namespace APPDATANS
{
	using namespace std;
	using namespace 	kapi::notify;
	using 		JSON	= kapi::JSON;
	using 	Context = ApplicationInterface::Context;
	const char dataPath[] = "/usr/lib/rsserial/appdata_files/data.json";


	void init()
	{

		handler(ApplicationInterface::EventTag, "appdata", [](Context C) {
			JSON request{C.request()};
			request.toFile("/tmp/appdatarx");
			JSON arg(C.parameter().c_str());
			JSON data{dataPath};
			JSON rsp{kapi::JSONType::JSON_OBJECT};

			rsp["status"] = 0;

			if (!data.good())
			{
				data = JSON{kapi::JSONType::JSON_OBJECT};
			}

			string method = request["method"].toString();
			if (method=="GET")
			{
				//rsp["status"] = 0;
				if (arg.exist("key")) {
					if (data.exist(arg["key"].toString())) {
						rsp["data"] = data[arg["key"].toString()];
					} else {
						rsp["data"] = JSON{kapi::JSONType::JSON_NULL};
					}
				} else {
					rsp["data"] = data;
				}
			} else if (method == "POST") {
				if (!arg.exist("key") || !arg.exist("value") ) {
					rsp["error"] = "need {key:value} " + arg.stringify();
				} else {
					//rsp["data"] = arg["key"].toString() + ":" + arg["value"].toString();
					data[arg["key"].toString()] = arg["value"].toString();
					data.toFile(dataPath);
					rsp["data"] = data[arg["key"].toString()];
				}
			} else {
				rsp["status"] = -1;
			}
			C.response(rsp.stringify());
		});
		return;
	}
}
}

void init()
{
	APPDATANS::init();
}
