;;
;;  ANSI COMMON LISP: 12. Numbers
;;

;;  byte
(deftest byte.1
  (typep (byte 10 20) 'lisp-system::bytespec)
  t)

(deftest byte-size.1
  (byte-size (byte 10 20))
  10)

(deftest byte-position.1
  (byte-position (byte 10 20))
  20)


;;  deposit-field
(deftest deposit-field.1
  (deposit-field 7 (byte 2 1) 0)
  6)

(deftest deposit-field.2
  (deposit-field -1 (byte 4 0) 0)
  15)

(deftest deposit-field.3
  (deposit-field 0 (byte 2 1) -3)
  -7)

(deftest deposit-field.4
  (deposit-field 7 (byte 0 1) 8)
  8)

(deftest deposit-field.5
  (deposit-field 7 (byte 0 1) -8)
  -8)

(deftest deposit-field-bound.1
  (deposit-field
    #xBE35989EAFAE284365A42B71D212B42166AA479ECDF1DF9FAD635438F
    (byte 40 80)
    #x632BFB8310BCB042EB7D5F25E)
  #x2B42166AA4B8310BCB042EB7D5F25E)

(deftest deposit-field-bound.2
  (deposit-field
    #x-806CD6DC605EED51A08700102482DFD93161E21A16D839044FFB0EE5D
    (byte 40 80)
    #x74D9ED3D870140ED72DEC337)
  #xD2026CE9E1ED3D870140ED72DEC337)

(deftest deposit-field-bound.3
  (deposit-field
    #x78822555B9D116C5AED1368FEF81FB4A4F76F8ACD4588B1B8FA5C1398
    (byte 40 80)
    #x-7E8842F389538AE7724AD97DC)
  #x-E04B5B08902F389538AE7724AD97DC)

(deftest deposit-field-bound.4
  (deposit-field
    #x-97FB39423BBAA7E0A29232B9507CD20605F513F350C76D24C495F4271
    (byte 40 80)
    #x-874EF4742B6E2CD7108201F9E)
  #x-CD20605F514742B6E2CD7108201F9E)

(deftest deposit-field-bound.5
  (deposit-field
    #x48EE4755BBFEA34BE2CB3C78F22F99527667B2BBF2D5B9319F243C799
    (byte 20 63)
    #xE9FDA62A63AD0122DF1522D1C)
  #xE9FDB2BBF3AD0122DF1522D1C)

(deftest deposit-field-bound.6
  (deposit-field
    #x-65082C8B2221C6DFF7E7B3337B2170E1316A24963635C0DFAB9351A7
    (byte 20 63)
    #xDD8A3D4388897D4CBC82712DF)
  #xDD8A5DB698897D4CBC82712DF)

(deftest deposit-field-bound.7
  (deposit-field
    #x331A714E9EC9CA299523FCE8E970820C49E44C4613940A103CF2939BD
    (byte 20 63)
    #x-60C7F204810709D898D5D63B7)
  #x-60C7B3B9E90709D898D5D63B7)

(deftest deposit-field-bound.8
  (deposit-field
    #x-BDBE56F7B60845D65DD74ADCEDD3B7317E49509C9CE2B0E17F93CB281
    (byte 20 63)
    #x-920C7AD1A5AA34BE8C6B45B8D)
  #x-920C509C9DAA34BE8C6B45B8D)

(deftest deposit-field-bound.9
  (deposit-field
    #x8BD0BD1F3EC527ED7331862BBE28852D4A88CB79AB0023BB76839D2FC
    (byte 20 64)
    #x2DF4AA3BF5D09F70772DE3F69)
  #x2DF4CB79A5D09F70772DE3F69)

(deftest deposit-field-bound.10
  (deposit-field
    #x-33AB25ECD672746B295D9CE502437229306BD27FC61B2DB8E7746390D
    (byte 20 64)
    #x4E658F97A21C2D8A5AFC3FEDA)
  #x4E652D80321C2D8A5AFC3FEDA)

(deftest deposit-field-bound.11
  (deposit-field
    #x33BBD69C3BF26703E63979B7840FC44B1D4392AC5499C8FD0AAF590B6
    (byte 20 64)
    #x-6357F587643708276D515D548)
  #x-63576D53A43708276D515D548)

(deftest deposit-field-bound.12
  (deposit-field
    #x-D4B094407722CF5BF5C470A41BCE50A82001EFEC6C94933897B6DA9A0
    (byte 20 64)
    #x-1BCE080FADD8E373128B58F8D)
  #x-1BCEEFEC6DD8E373128B58F8D)

(deftest deposit-field-bound.13
  (deposit-field
    #x735A719B220AC1F45069F58B836E637B37A972F081E5C1205348751D7
    (byte 20 65)
    #x1E891341599D9457A27CEE4A8)
  #x1E8972F0999D9457A27CEE4A8)

(deftest deposit-field-bound.14
  (deposit-field
    #x-62938947ABC9BFB6F10CBE45B562A92C9F96CC23A2C83E20C3B6CA975
    (byte 20 65)
    #xA835C5925159FF0A0BCFE920F)
  #xA83533DC5159FF0A0BCFE920F)

(deftest deposit-field-bound.15
  (deposit-field
    #x1C53C1AB892D869799259C83804321151806D9E4D5BEFA898116F29D6
    (byte 20 65)
    #x-850969CF901CE270449FED4F9)
  #x-8509261B301CE270449FED4F9)

(deftest deposit-field-bound.16
  (deposit-field
    #x-16CE470F73B940C2F69BA86BFF0291AC0F2DECD35F78986737A662F92
    (byte 20 65)
    #x-882083F91B17A129C13F0917A)
  #x-8821ECD35B17A129C13F0917A)

(deftest deposit-field-bound.17
  (deposit-field
    #xD59750DB7B39E45C770FB525456FC9B1C261E0E9803073FC92F8CC5C0
    (byte 80 65)
    #xB69860E43814D90FC7D4FEE15)
  #x1525456FC9B1C261E0E99814D90FC7D4FEE15)

(deftest deposit-field-bound.18
  (deposit-field
    #x-AE862093A478B39448AE24B6F67237B9E2180FAD81ECC3D25A6DCC82A
    (byte 80 65)
    #x486814B7BE86DD73E1D1496DB)
  #x1B49098DC8461DE7F0527E86DD73E1D1496DB)

(deftest deposit-field-bound.19
  (deposit-field
    #x8E20E6C0702F26185F13FF36E2D241E57FA98321AD1A1BEC00E2EE54C
    (byte 80 65)
    #x-FEE27E074BAF1A8C5EF081009)
  #x-C91D2DBE1A80567CDE4BAF1A8C5EF081009)

(deftest deposit-field-bound.20
  (deposit-field
    #x-7EEDB2F82B69EF77C87C11F7A67B505F190EB231F8FCDF76CC061F5B6
    (byte 80 65)
    #x-D92D0C3E6ADF10B046128449F)
  #x-11F7A67B505F190EB231EADF10B046128449F)

(deftest deposit-field-bound.21
  (deposit-field
    #xC138EF6C27C6B1D01C4F66F4D6DFC9C9C51DEC914A26233FB45AADB59
    (byte 63 65)
    #x2C8C9302DEE5EED58B6660996)
  #x6DFC9C9C51DEC915EE5EED58B6660996)

(deftest deposit-field-bound.22
  (deposit-field
    #x-9ED647D7562DF1477C88128AD713279969EEF94974A74299FC8E5BD68
    (byte 63 65)
    #xEE774BCD1433C4EBD47140B21)
  #x8ECD866961106B69433C4EBD47140B21)

(deftest deposit-field-bound.23
  (deposit-field
    #xE959B237D00A5F1A21F169089B76D5D55E47B0B5DD119A4B19F8F2FF9
    (byte 63 65)
    #x-9FCA55A6CE395229CDF6069BE)
  #x-4892A2AA1B84F4A2E395229CDF6069BE)

(deftest deposit-field-bound.24
  (deposit-field
    #x-694A87B9922F83A8E2892EFB6936A45CEB1094413E774568E32C3AB9E
    (byte 63 65)
    #x-27E88721CE2221331BF819480)
  #x-936A45CEB1094412E2221331BF819480)

(deftest deposit-field-bound.25
  (deposit-field
    #x93BBC6F472C24DB6647169DF9B304AEE9837D465A1ADEE8C64F431A1
    (byte 62 65)
    #x1618DD77B21ED6917D1FB4EB7)
  #x1B304AEE9837D46521ED6917D1FB4EB7)

(deftest deposit-field-bound.26
  (deposit-field
    #x-58642EFA84FC4520CB64D0B760C7F253DBCDC79D3C960939CD0DB8B30
    (byte 62 65)
    #x48B7676F9F929F7C368AF21FB)
  #x7380DAC24323862DF929F7C368AF21FB)

(deftest deposit-field-bound.27
  (deposit-field
    #xC7ADBEDA3E08B0E62B21052B0EB42995BA4592FC0A921739E92F96591
    (byte 62 65)
    #x-CA6E25A6BE204C3B5A22853BE)
  #x-14BD66A45BA6D03FE204C3B5A22853BE)

(deftest deposit-field-bound.28
  (deposit-field
    #x-783E6E8648E73B05167646139E3655FFD7AC1122511E278938D4A2729
    (byte 62 65)
    #x-1E4283E8E11E4858EA3FDFCD4)
  #x-63655FFD7AC1122411E4858EA3FDFCD4)

(deftest deposit-field-bound.29
  (deposit-field
    #x424ECBF4C0292A04CCA411B607D432D66EE00B2CD89EB8FC6DB0A5592
    (byte 63 65)
    #x3735A0EEF27C8101B451A9D3F)
  #x7D432D66EE00B2CD27C8101B451A9D3F)

(deftest deposit-field-bound.30
  (deposit-field
    #x-2D9B3BF54FDF93F6962C6954B92C4CCAC8ED80A6B8649C76CD5C3C882
    (byte 63 65)
    #x93D8D3FF065DF86B32A2E800F)
  #x6D3B33537127F59465DF86B32A2E800F)

(deftest deposit-field-bound.31
  (deposit-field
    #x7E477D885C85ED210B447E7B38839C4A63061FA843FB049473CBEA8B5
    (byte 63 65)
    #x-B828B1CA479D9A37B63D22FF4)
  #x-77C63B59CF9E057A79D9A37B63D22FF4)

(deftest deposit-field-bound.32
  (deposit-field
    #x-9D35920F38D8205FD19F1CF94824E3DC7DC6608D391A40E7E64AC80ED
    (byte 63 65)
    #x-AA47DE624F9E55E9F81E937CB)
  #x-824E3DC7DC6608D2F9E55E9F81E937CB)

(deftest deposit-field-bound.33
  (deposit-field
    #x85E14506A3F5DDEBEE704B47898C8D0C55D0730BD1CFC493CE8F7A9FB
    (byte 256 0)
    #xD7328FAA278FFCFA3E9208066)
  #x85E14506A3F5DDEBEE704B47898C8D0C55D0730BD1CFC493CE8F7A9FB)

(deftest deposit-field-bound.34
  (deposit-field
    #x-E7DF652542D3D2084BA37A9F5E83508A219DF5E116C273138B970A941
    (byte 256 0)
    #x9304FCC9D0F071D1BD195F951)
  #xFFFFFFF18209ADABD2C2DF7B45C8560A17CAF75DE620A1EE93D8CEC7468F56BF)

(deftest deposit-field-bound.35
  (deposit-field
    #x4AF7DD852E870AD0AF11FB6F5D4A6191F129F79272CE53EF38B2B44CB
    (byte 256 0)
    #x-22F9139D4D571701C185963B1)
  #x-FFFFFFFB508227AD178F52F50EE0490A2B59E6E0ED6086D8D31AC10C74D4BB35)

(deftest deposit-field-bound.36
  (deposit-field
    #x-78360463E898C05078F042511D3E720C120E7649D08A2C6BA7C480B7
    (byte 256 0)
    #x-26FE87B31894D74CE89AA4E21)
  #x-78360463E898C05078F042511D3E720C120E7649D08A2C6BA7C480B7)


;;  dpb
(deftest dpb.1
  (dpb 1 (byte 1 10) 0)
  1024)

(deftest dpb.2
  (dpb -2 (byte 2 10) 0)
  2048)

(deftest dpb.3
  (dpb 1 (byte 2 10) 2048)
  1024)

(deftest dpb.4
  (dpb 4 (byte 0 10) 10)
  10)

(deftest dpb.5
  (dpb 4 (byte 0 10) -10)
  -10)

(deftest dpb-bound.1
  (dpb
    #xD94E8136809C1F3D4DC54C3F298F7279289A5C0DE223A16F83C073B33
    (byte 40 80)
    #xDBEFEB496AD1C33F4981C996A)
  #xF83C073B33B496AD1C33F4981C996A)

(deftest dpb-bound.2
  (dpb
    #x-287D41FA8B98E6E7D2454625045B2898B01D529DD6E146C3DB0DB403
    (byte 40 80)
    #x7FF571295E9FC67DFD1E2CD63)
  #x3C24F24BFD1295E9FC67DFD1E2CD63)

(deftest dpb-bound.3
  (dpb
    #xBFD19B405A5EE805677BC84ADA524BAFE91B759406D086AE5224F0D1B
    (byte 40 80)
    #x-B2F6895884999F1F5A5DA60B2)
  #x-1ADDB0F2E495884999F1F5A5DA60B2)

(deftest dpb-bound.4
  (dpb
    #x-33A217A2CA993821F1CAEDCCBAA9C44847C3B3FAC31C47F37A05DDC26
    (byte 40 80)
    #x-B8247A02A8CF81D28DEF1D01)
  #x-37A05DDC257A02A8CF81D28DEF1D01)

(deftest dpb-bound.5
  (dpb
    #xB014640F042C32EF5FC5F42A50E03FCAC8DB90EDDFDC3D7C875F69B00
    (byte 20 63)
    #x745A2F81E872DC0FEC92B3977)
  #x745A34D80072DC0FEC92B3977)

(deftest dpb-bound.6
  (dpb
    #x-A39D5BFD3D9F6E848543906E8AA5712FA24DEC227CDA644F2FF043F77
    (byte 20 63)
    #x1C06A5D37D759303E21156C6)
  #x1C05E044FD759303E21156C6)

(deftest dpb-bound.7
  (dpb
    #x7D309ADAA67A24BE4D4AC14712608AEA6F3DC3EEF2C72F4C671C01E9E
    (byte 20 63)
    #x-A257C1B90A6ADFF317E626397)
  #x-A257FF0B0A6ADFF317E626397)

(deftest dpb-bound.8
  (dpb
    #x-317603BBBF0DA7CF62ED06A0E2F1C341ECBC70FC121BA3E36C0DAFC56
    (byte 20 63)
    #x-337E68268F2CC428AB090F37F)
  #x-337E57E2AF2CC428AB090F37F)

(deftest dpb-bound.9
  (dpb
    #xE363226B9A1A9E58B191D587E06CB0FB0D0EDF538424D046ED1FEC4CA
    (byte 20 64)
    #xBD9356C587F0B65267F549A50)
  #xBD93EC4CA7F0B65267F549A50)

(deftest dpb-bound.10
  (dpb
    #x-559D98DF1B5B89EA78F5D743CBF614D118E9AAA55EB8BFE7C4E0794BF
    (byte 20 64)
    #xC8FA2CC400524AC6D17C34DFC)
  #xC8FA86B410524AC6D17C34DFC)

(deftest dpb-bound.11
  (dpb
    #x4E456959A6EF2EFCDDC217571E92A75DF2CCD6161989A13F512DB49F1
    (byte 20 64)
    #x-538430B5E23104158E51D6773)
  #x-53844B60E23104158E51D6773)

(deftest dpb-bound.12
  (dpb
    #x-FC0F3A15E38E18107D0E5E71C6B3927FAAEC5BE416118391F92375874
    (byte 20 64)
    #x-F5FF7626F6F7420896F0985A0)
  #x-F5FF758736F7420896F0985A0)

(deftest dpb-bound.13
  (dpb
    #xB7FA6B16D4AFC56C120658CA6C2BA2CBB0FDA1B768259C1621C12B4F8
    (byte 20 65)
    #x7885711526DFA1F2E1891A97B)
  #x7884569F06DFA1F2E1891A97B)

(deftest dpb-bound.14
  (dpb
    #x-2A2DAA8AEE63E2925DF8AA8BA79BE67F909B5B31FAAE84CFFF3CBA22F
    (byte 20 65)
    #x87DB1BC1CE848560A65B911E7)
  #x87DA8BBA2E848560A65B911E7)

(deftest dpb-bound.15
  (dpb
    #x8FB673EF45972BF010CE2B1E22D99991E5AE27CEB4F60B5C75694EA13
    (byte 20 65)
    #x-9BD76518C5B4B7C8FB1AF87BA)
  #x-9BD762BD85B4B7C8FB1AF87BA)

(deftest dpb-bound.16
  (dpb
    #x-3436ED463CDD6857649C47574AEEB94C59283EBE33A071945746D3812
    (byte 20 65)
    #x-8E06A8CD8FAAA7887C20E31A6)
  #x-8E07A7022FAAA7887C20E31A6)

(deftest dpb-bound.17
  (dpb
    #x62A48CFF0EB3E8663BBB0719787622D3FCC82FBC75B8D4C0F15E5059C
    (byte 80 65)
    #xF34A3615D7EA7ADB775E50504)
  #x1F78EB71A981E2BCA0B397EA7ADB775E50504)

(deftest dpb-bound.18
  (dpb
    #x-274BD0ED328F44A55BB227441D54E711875A5D84DD335DA619E29877E
    (byte 80 65)
    #x2193682957DC62520E7B76F2D)
  #x4F6459944B3CC3ACF1057DC62520E7B76F2D)

(deftest dpb-bound.19
  (dpb
    #x5E31641B234F39092C4F649EC64DAE71DBF1B3CBDE2C11BE32DAFFC4A
    (byte 80 65)
    #x-C682083CA5BC4DEBF25F2B29D)
  #x-186843A7DC839A4A0076A5BC4DEBF25F2B29D)

(deftest dpb-bound.20
  (dpb
    #x-A1C069B3FE8C3B12FE2E7DE6D34875125F823C9C89D26C1A7B0602AE6
    (byte 80 65)
    #x-94BB5FF4DE7E8D929DDFF3643)
  #x-193913A4D834F60C055CBE7E8D929DDFF3643)

(deftest dpb-bound.21
  (dpb
    #x53CE9D63B0FD389E13DB915CEC24C888D1311FA8C6A63D7146AE388D4
    (byte 63 65)
    #xBD292EC2BA3F6A994A831D9E1)
  #xD4C7AE28D5C711A9A3F6A994A831D9E1)

(deftest dpb-bound.22
  (dpb
    #x-EA8D5C87B74443E8087366B376CF9202A80915F58CAA774B64563E5BB
    (byte 63 65)
    #x8376DE7E6B298EB9FF305F4CB)
  #x6AB116937538348AB298EB9FF305F4CB)

(deftest dpb-bound.23
  (dpb
    #x98B6B076118175A7DBC8F3039CF80A56EDCF0F23C321FDDA72BCB884A
    (byte 63 65)
    #x-9FEEB9059CDC981224E5BF789)
  #x-9BC044B1A868EF6BCDC981224E5BF789)

(deftest dpb-bound.24
  (dpb
    #x-7CC867DF208C5B0677E37BAD8F230BE3D0623695A8AB0E3035FE9261A
    (byte 63 65)
    #x-3A8A23BE141D78D5D972661E)
  #x-1561C606BFD24C32141D78D5D972661E)

(deftest dpb-bound.25
  (dpb
    #xED3A6D3B0C3842FA8387BA7D51FB5C1ADACD615B083C9EBB7157101B4
    (byte 62 65)
    #x5FBC2D59756ABD79DF291F297)
  #x793D76E2AE2036956ABD79DF291F297)

(deftest dpb-bound.26
  (dpb
    #x-5D37362CACFFC2FA2FEA8F9AA6552D5374ADEF5500B92E20F50793D08
    (byte 62 65)
    #x93E42DF1B523D2EBF9A5DB69D)
  #x68DA3BE15F0D85F1523D2EBF9A5DB69D)

(deftest dpb-bound.27
  (dpb
    #x4DA7C7F5FCF221B7DC9707545A9BED7540FF1A781308A59CC030D4544
    (byte 62 65)
    #x-4638F81B63A11693ADB7961EC)
  #x-1EEB4C67F9E575763A11693ADB7961EC)

(deftest dpb-bound.28
  (dpb
    #x-A4C47B15106C3DA8EF6DB0987F172C735821CA52EFE148C4F3BEAB9CB
    (byte 62 65)
    #x-847A875A9D823371198E35864)
  #x-7C29189E77D57395D823371198E35864)

(deftest dpb-bound.29
  (dpb
    #x19D98CFCA5B04412B9D49B7464E7A962C131DC40C243C519FC4A1FE5C
    (byte 63 65)
    #x48A628797F49F9FFAC1BDD704)
  #x4878A33F8943FCB9F49F9FFAC1BDD704)

(deftest dpb-bound.30
  (dpb
    #x-BA60D201FBEADF82CA0A106960D1800459322ABD50B4E8DAD24C21247
    (byte 63 65)
    #x2CA812FECF3172C44D6CAE3A1)
  #xE962E4A5B67BDB72F3172C44D6CAE3A1)

(deftest dpb-bound.31
  (dpb
    #xF04375518B2427CC3699B6ADD0A5D639C5F2B49AD09D35CF1043B8DFA
    (byte 63 65)
    #x-C080251F79B47667F2017617A)
  #x-EC59461DF788E40B9B47667F2017617A)

(deftest dpb-bound.32
  (dpb
    #x-1739E69BF9AF9322D997ED593ED76F0B96A94503DE851EC03E480EE83
    (byte 63 65)
    #x-38EDCE8CFFE8563E2B3BE43B6)
  #x-D0A3D807C901DD05FE8563E2B3BE43B6)

(deftest dpb-bound.33
  (dpb
    #xCBD016B38F82658107D8C9C8992B28836427242A4090005863A5F4DA
    (byte 256 0)
    #x15020C494F1446E699E3C9F7)
  #xCBD016B38F82658107D8C9C8992B28836427242A4090005863A5F4DA)

(deftest dpb-bound.34
  (dpb
    #x-24A7AF901A10EA08CB049ACCBB700B594486FE1526FE65D430CF671D6
    (byte 256 0)
    #x44F205D10DB2E1089B0E9E84D)
  #xFFFFFFFDB58506FE5EF15F734FB6533448FF4A6BB7901EAD9019A2BCF3098E2A)

(deftest dpb-bound.35
  (dpb
    #x5A9D3B3AA69C4840F20BEA3E7993C846B290650098B232677627433F8
    (byte 256 0)
    #x-A4F631B8836DCF747FA0B8E34)
  #x-FFFFFFFA562C4C55963B7BF0DF415C1866C37B94D6F9AFF674DCD9889D8BCC08)

(deftest dpb-bound.36
  (dpb
    #x-1F3398BBB7F22EF90D5252E3BF9BC4899B0B04CCF3FF6C458E5CE3B39
    (byte 256 0)
    #x-62BE7199C0E228AF106C61F08)
  #x-1F3398BBB7F22EF90D5252E3BF9BC4899B0B04CCF3FF6C458E5CE3B39)


;;  ldb
(deftest ldb.1
  (ldb (byte 2 1) 10)
  1)

(deftest ldb.2
  (ldb (byte 2 1) 8)
  0)

(deftest ldb-bound.1
  (ldb (byte 40 80)
       #x565F19B1732B74A339C1A5FBFB2C69CDAC529CC956E75C760A87B9062)
  #xC69CDAC529)

(deftest ldb-bound.2
  (ldb (byte 40 80)
       #x-EA2BC5DEECA4D85AB86C549A15807271C80CB954E68F779A43201B114)
  #xF8D8E37F34)

(deftest ldb-bound.3
  (ldb (byte 20 63)
       #xD9EC8919D034420AD1415BEA463F9236D47A91AD53E846886E78F8086)
  #x235AA)

(deftest ldb-bound.4
  (ldb (byte 20 63)
       #x-BC52122073057083CF5524A51F33359CB69141939C3B9A06754061838)
  #x7CD8C)

(deftest ldb-bound.5
  (ldb (byte 20 64)
       #x4DC2BA69C5A703E2B53891726F6C684C0C5BC214C159EBC4318FD904A)
  #xC214C)

(deftest ldb-bound.6
  (ldb (byte 20 64)
       #x-5FAFEEF2055A20122CBF7FE2D986B8225A65C41FFE5595DC46ECB2F8C)
  #x3BE00)

(deftest ldb-bound.7
  (ldb (byte 20 65)
       #x78FE17934BA6CD1126E9637EE7F25C14654302ED9D3E4C61E4705F68E)
  #x8176C)

(deftest ldb-bound.8
  (ldb (byte 20 65)
       #x-893D7F9F0E5AB30C731EC99E8FFDA0D7FDFE1594003B58607273010B9)
  #xF535F)

(deftest ldb-bound.9
  (ldb (byte 80 65)
       #x1B1D6BB4AC55ED171AB3E1F7F2A000E24599C7E886B3F5B8D5AAABC40)
  #xFBF950007122CCE3F44)

(deftest ldb-bound.10
  (ldb (byte 80 65)
       #x-4BB847E83E5FDA2F40B894B3742D7299755C91E4B20B8830AEDD503B8)
  #x5A645E946B34551B70DA)

(deftest ldb-bound.11
  (ldb (byte 63 65)
       #x72F32AAE7195D604D7B9988FD07A60EA05383EFCF2A2E002FF0B28AAB)
  #x3D3075029C1F7E7)

(deftest ldb-bound.12
  (ldb (byte 63 65)
       #x-388C046B56FBBB2EB2B572DBDF14A45AE4867A5212B811B198DF3F539)
  #x75ADD28DBCC2D6F)

(deftest ldb-bound.13
  (ldb (byte 62 65)
       #xA9B6F28DD2304269E24993BA558A74D42710859A7233B315E079FB693)
  #x2C53A6A138842CD3)

(deftest ldb-bound.14
  (ldb (byte 62 65)
       #x-D1C3F580B9FF9A8FB2183B45FD15964226E207CE544EA9C4B3BCBB58A)
  #x17534DEEC8EFC18D)

(deftest ldb-bound.15
  (ldb (byte 63 65)
       #x97DDAB61B67E68F8B6A87213C1EB10D9B5B3D00ED815543E27FDFFFD9)
  #xF5886CDAD9E8076)

(deftest ldb-bound.16
  (ldb (byte 63 65)
       #x-2C6274B56CC89FD0AB7DFD882D1BC3EC8CFAE59E22E8189F434994358)
  #x1721E09B9828D30E)

(deftest ldb-bound.17
  (ldb (byte 256 0)
       #x57E678C7C4C1A66242A4F8EA866614D3B7E11026B4A496A0227A5ABD4)
  #x57E678C7C4C1A66242A4F8EA866614D3B7E11026B4A496A0227A5ABD4)

(deftest ldb-bound.18
  (ldb (byte 256 0)
       #x-16F7997FF9F4116C90598FF3F1618C8D2A714C23DBD1CD51BD30F90A3)
  #xFFFFFFFE9086680060BEE936FA6700C0E9E7372D58EB3DC242E32AE42CF06F5D)

(deftest ldb-bound.19
  (ldb (byte 256 64)
       #xF9854E06859C3182F9C53D36BD2B1D567FFA795252888402111577413)
  #xF9854E06859C3182F9C53D36BD2B1D567FFA79525)

(deftest ldb-bound.20
  (ldb (byte 256 64)
       #x-399E77CECFEADEF536ED9AB535FB1D0DFCFB86635C157E38DA6B05167)
  #xFFFFFFFFFFFFFFFFFFFFFFFC66188313015210AC912654ACA04E2F20304799CA)


;;  (setf ldb)
(deftest setf-ldb.1
  (let ((a (list 8)))
    (values
      (setf (ldb (byte 2 1) (car a)) 1)
      a))
  1 (10))

(deftest setf-ldb.2
  (let ((a (list 8)))
    (values
      (setf (ldb (byte 2 1) (car a)) #xFFFFFF)
      a))
  #xFFFFFF (14))


;; ldb-test
(deftest ldb-test.1
  (ldb-test (byte 4 1) 16)
  t)

(deftest ldb-test.2
  (ldb-test (byte 3 1) 16)
  nil)

(deftest ldb-test.3
  (ldb-test (byte 3 2) 16)
  t)

(deftest ldb-test.4
  (ldb-test (byte 1 100) 5)
  nil)

(deftest ldb-test.5
  (ldb-test (byte 1 100) -5)
  t)

(deftest ldb-test.6
  (ldb-test (byte 4 100) #x10000000000000000000000000000000000000000000000000)
  nil)

(deftest ldb-test.7
  (ldb-test (byte 4 100) #x-10000000000000000000000000000000000000000000000000)
  nil)


;;  mask-field
(deftest mask-field.1
  (mask-field (byte 1 5) -1)
  32)

(deftest mask-field-bound.1
  (mask-field (byte 40 80)
              #x490BA0C502B32EC902B7D169EBB8ACC99A68C01FF56BCEB151340E5D2)
  #x8ACC99A68C00000000000000000000)

(deftest mask-field-bound.2
  (mask-field (byte 40 80)
              #x-2C304BE8DBBF7557162DD934B014291791FA19BE865DCAF6AEA9FEDA7)
  #xBD6E86E05E00000000000000000000)

(deftest mask-field-bound.3
  (mask-field (byte 20 63)
              #xE98BDD57F3EC6523CE8895A6A47F09B869A5E43C3BBD4D4A1B8A16150)
  #x643C38000000000000000)

(deftest mask-field-bound.4
  (mask-field (byte 20 63)
              #x-5F9B35F0BB9E69706FCC1970720CA4C2D2943E81AC54DCDE585D34F46)
  #x417E50000000000000000)

(deftest mask-field-bound.5
  (mask-field (byte 20 64)
              #x883F530EB2EB5BFC55CF1C55F1796D3B5B7D2EF81F5E21A37724F66D9)
  #x2EF810000000000000000)

(deftest mask-field-bound.6
  (mask-field (byte 20 64)
              #x-A9BA99F77BA276199CB215F0CE5B90E6E96F85A497DE0E8CA51ACB7E0)
  #x7A5B60000000000000000)

(deftest mask-field-bound.7
  (mask-field (byte 20 65)
              #xD5F0EE99BA4720288C9BDA3900A822C29F5C662D345F8066BE4DAF7BB)
  #x662D20000000000000000)

(deftest mask-field-bound.8
  (mask-field (byte 20 65)
              #x-28E110A4F0E08C545970938F20885D7996850B7881AB49BE6B4EFA1E9)
  #xF48760000000000000000)

(deftest mask-field-bound.9
  (mask-field (byte 80 65)
              #x2BCD99F26A522F8460ABF2790D468D7F16CD164D295E6EBAE12C286E4)
  #x12790D468D7F16CD164D20000000000000000)

(deftest mask-field-bound.10
  (mask-field (byte 80 65)
              #x-F060F778F8AFA09217D4E2F7A513DCF42C38D62AE3E030390B2CC7FA6)
  #x1D085AEC230BD3C729D500000000000000000)

(deftest mask-field-bound.11
  (mask-field (byte 63 65)
              #xA4EB143BF756ECE8C46FDCE97F5F966B5FE9A30AB767FB423E019A984)
  #xF5F966B5FE9A30AA0000000000000000)

(deftest mask-field-bound.12
  (mask-field (byte 63 65)
              #x-FB206726F377A9F6B5A171E057C8F60E81EF4907B926B09E3B71C7211)
  #x83709F17E10B6F840000000000000000)

(deftest mask-field-bound.13
  (mask-field (byte 62 65)
              #xB104CED5B8EA28738E081EB08096AE7399C85AC79F04ED0D8FE3A2DA0)
  #x96AE7399C85AC780000000000000000)

(deftest mask-field-bound.14
  (mask-field (byte 62 65)
              #x-F6E330814CBB87D9FA958F4DEA4561D895537855E9D9A4AAA5259E817)
  #x5BA9E276AAC87AA00000000000000000)

(deftest mask-field-bound.15
  (mask-field (byte 63 65)
              #xE0D78068A36817F2278F67502F93878584DFDC23C3795EBDD13DDC0D4)
  #xF93878584DFDC23C0000000000000000)

(deftest mask-field-bound.16
  (mask-field (byte 63 65)
              #x-E36B167EEF42737F827B8523900FBB6760DD53AFB738BF8393B5EAF9A)
  #xFF044989F22AC5040000000000000000)

(deftest mask-field-bound.17
  (mask-field (byte 256 0)
              #xE8AA10ACC5D27649E13B435753EE145CFEB16B196073B0A910FCA26CC)
  #xE8AA10ACC5D27649E13B435753EE145CFEB16B196073B0A910FCA26CC)

(deftest mask-field-bound.18
  (mask-field (byte 256 0)
              #x-FC4E6583792874205FE09F08CBAB2D8588343E1ED5D7643F284521286)
  #xFFFFFFF03B19A7C86D78BDFA01F60F73454D27A77CBC1E12A289BC0D7BADED7A)

(deftest mask-field-bound.19
  (mask-field (byte 256 64)
              #xFE959802DA09DD69C3C411A835A6CDEEC24CF7765087D15A9E2802591)
  #xFE959802DA09DD69C3C411A835A6CDEEC24CF77650000000000000000)

(deftest mask-field-bound.20
  (mask-field (byte 256 64)
              #x-9C7F7814F2EB487F522858A416069486BB0E734385FAC526C4862FE1E)
  #xFFFFFFFFFFFFFFFFFFFFFFF638087EB0D14B780ADD7A75BE9F96B7944F18CBC70000000000000000)


;;  (setf mask-field)
(deftest setf-mask-field.1
  (let ((a (list 15)))
    (values
      (setf (mask-field (byte 2 0) (car a)) 1)
      a))
  1 (13))

(deftest setf-mask-field.2
  (let ((a (list 15)))
    (values
      (setf (mask-field (byte 2 0) (car a)) #xFFFFFF)
      a))
  #xFFFFFF (15))
