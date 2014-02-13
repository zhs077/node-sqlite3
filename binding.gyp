{
  'targets': [
    {
      'target_name': 'node_sqlite3',
     'sources': [
					'src/third_party/CodingConv/encodeutil.cpp','src/nodejs/Sqlite3Client.cpp'
					,'src/nodejs/Sqlite3Connection.cpp','src/nodejs/QueryJob.cc','src/nodejs/Statement.cpp','src/nodejs/NodeFunction.cpp',
					 'src/sqlite3_encrypt/DbSqlite.cpp','src/sqlite3_encrypt/sqlite3/sqlite3secure.c'
				],
	'conditions':[
				["OS=='win'",{
				 'defines': ['UNICODE']
			
								}
				]

				],
'include_dirs':[
              'src/sqlite3_encrypt'
			  ,'src/third_party/CodingConv'
				
            ],
	
		'cflags':['-fexceptions'],
	'cflags_cc':['-fexceptions']

	

    }
  ]

}
