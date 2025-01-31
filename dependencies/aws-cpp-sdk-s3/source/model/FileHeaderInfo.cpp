﻿/*
* Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/

#include <aws/s3/model/FileHeaderInfo.h>
#include <aws/core/utils/HashingUtils.h>
#include <aws/core/Globals.h>
#include <aws/core/utils/EnumParseOverflowContainer.h>

using namespace Aws::Utils;

// DV Edit:
#undef IGNORE


namespace Aws
{
  namespace S3
  {
    namespace Model
    {
      namespace FileHeaderInfoMapper
      {

        static const int USE_HASH = HashingUtils::HashString("USE");
        static const int IGNORE_HASH = HashingUtils::HashString("IGNORE");
        static const int NONE_HASH = HashingUtils::HashString("NONE");


        FileHeaderInfo GetFileHeaderInfoForName(const Aws::String& name)
        {
          int hashCode = HashingUtils::HashString(name.c_str());
          if (hashCode == USE_HASH)
          {
            return FileHeaderInfo::USE;
          }
          else if (hashCode == IGNORE_HASH)
          {
            return FileHeaderInfo::IGNORE;
          }
          else if (hashCode == NONE_HASH)
          {
            return FileHeaderInfo::NONE;
          }
          EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
          if(overflowContainer)
          {
            overflowContainer->StoreOverflow(hashCode, name);
            return static_cast<FileHeaderInfo>(hashCode);
          }

          return FileHeaderInfo::NOT_SET;
        }

        Aws::String GetNameForFileHeaderInfo(FileHeaderInfo enumValue)
        {
          switch(enumValue)
          {
          case FileHeaderInfo::USE:
            return "USE";
          case FileHeaderInfo::IGNORE:
            return "IGNORE";
          case FileHeaderInfo::NONE:
            return "NONE";
          default:
            EnumParseOverflowContainer* overflowContainer = Aws::GetEnumOverflowContainer();
            if(overflowContainer)
            {
              return overflowContainer->RetrieveOverflow(static_cast<int>(enumValue));
            }

            return {};
          }
        }

      } // namespace FileHeaderInfoMapper
    } // namespace Model
  } // namespace S3
} // namespace Aws
