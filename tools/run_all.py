from pathlib import Path
import subprocess,json,sys,shutil,statistics
R=Path(__file__).resolve().parents[1];B=R/'build';B.mkdir(exist_ok=True)
commands=[]
def command(args,expected=0,input=None):
 r=subprocess.run(args,cwd=R,text=True,input=input,capture_output=True)
 commands.append({'command':args,'exit_code':r.returncode,'expected_exit':expected})
 if r.returncode!=expected:raise RuntimeError(' '.join(args)+'\n'+r.stdout+r.stderr)
 return r.stdout+r.stderr
base=['gcc','-std=c17','-Wall','-Wextra','-Wpedantic','-O2']
def compile(name,sources,flags=[]):
 exe=B/name
 output=command(base+flags+sources+['-o',str(exe)])
 if output:
  print('DIAGNOSTIC',name,output)
  (R/Path(sources[0]).parent/(name+'_warnings.txt')).write_text(output)
 return exe
def run(task,filename,exe,args=[],input=None,expected=0):
 out=command([str(exe),*args],expected,input)
 (R/task/filename).write_text(out);print(task,filename,': PASS',flush=True);return out
common=['common/billing.c']
(R/'Task07/precision.c').write_text((R/'Task10/totals.c').read_text())
for n,source,extra in [(1,'data_demo.c',common),(2,'types.c',[]),(3,'complete_code.c',[]),(4,'macros.c',[]),(5,'header_demo.c',common),(7,'conversions.c',[]),(11,'recommendation.c',common)]:
 task=f'Task{n:02}';e=compile(f'task{n}',[task+'/'+source]+extra);run(task,'output.txt',e)
e=compile('task2_complete',['Task02/complete_code.c']);out=run('Task02','complete_code_output.txt',e)
assert 'Consumption: 330 units' in out and 'Consumption: 256 units' in out
original=(R/'Task03/complete_code.c').read_text();(R/'Task03/revised_rate.c').write_text(original.replace('#define RATE2_PAISE 225','#define RATE2_PAISE 230'))
e=compile('task3_revised',['Task03/revised_rate.c']);out=run('Task03','revised_output.txt',e);assert '150 units -> Rs 165.00' in out and '350 units -> Rs 955.00' in out
pre=command(['gcc','-E','-P','Task04/macros.c']);(R/'Task04/preprocessed.i').write_text(pre)
e=compile('task5_debug',['Task05/header_demo.c',*common],['-DDEBUG']);out=run('Task05','debug_output.txt',e);assert 'DEBUG units=150 energy=11250 fixed=5000 total=16250' in out
print('Header DEBUG-on/off assertions passed')
e=compile('task6',['Task06/scanf_demo.c']);
for name,data,code in [('valid','1200 1530\n',0),('letters','hello 1530\n',2),('negative','-1 1530\n',2)]:
 (R/'Task06'/f'{name}.txt').write_text(data);run('Task06',f'{name}_output.txt',e,input=data,expected=code)
wrong=command(base+['-Werror=format','-c','Task06/wrong_format.c','-o',str(B/'wrong.o')],expected=1)
(R/'Task06/format_diagnostic.txt').write_text(wrong)
# Record the compiler pipeline, with binary products kept in the local build directory.
command(['gcc','-E','Task08/stages.c','-o',str(R/'Task08/stages.i')])
command(['gcc','-std=c17','-Wall','-Wextra','-S','Task08/stages.c','-o',str(R/'Task08/stages.s')])
command(['gcc','-std=c17','-Wall','-Wextra','-c','Task08/stages.c','-o',str(B/'stages.o')])
command(['gcc',str(B/'stages.o'),'-o',str(B/'stages')]);run('Task08','output.txt',B/'stages')
(R/'Task08/stage_sizes.json').write_text(json.dumps({p.name:p.stat().st_size for p in [R/'Task08/stages.c',R/'Task08/stages.i',R/'Task08/stages.s',B/'stages.o',B/'stages']},indent=2))
for name in ['compile_error','link_error']:
 out=command(base+[f'Task08/{name}.c','-o',str(B/name)],expected=1);(R/'Task08'/f'{name}_diagnostic.txt').write_text(out)
e=compile('runtime',['Task08/runtime_error.c']);run('Task08','runtime_error_output.txt',e,expected=2)
sources=['Task09/main.c','Task09/input.c',*common]
e=compile('billing',sources)
out=run('Task09','valid_output.txt',e,input=(R/'Task09/input_valid.txt').read_text());assert 'total=Rs 1487.50' in out
run('Task09','invalid_output.txt',e,input=(R/'Task09/input_invalid.txt').read_text(),expected=2)
e=compile('tests',['Task09/tests.c','Task09/input.c',*common]);run('Task09','tests_output.txt',e)
# Sanitizers check valid and invalid parsing/billing paths for undefined behaviour.
e=compile('tests_sanitized',['Task09/tests.c','Task09/input.c',*common],['-O1','-fsanitize=address,undefined','-fno-omit-frame-pointer'])
run('Task09','sanitizer_output.txt',e)
for task,file in [('Task07','precision.c'),('Task10','totals.c')]:
 e=compile(task.lower()+'_precision',[task+'/'+file,*common]);runs=[]
 for i in range(3):runs.append(command([str(e)]))
 (R/task/'precision_runs.json').write_text(json.dumps(runs,indent=2));(R/task/'precision_output.txt').write_text(runs[0])
# Separate compilation and relinking also demonstrate the alternative build design.
for src in sources:
 command(base+['-c',src,'-o',str(B/(Path(src).stem+'.o'))])
command(['gcc',*[str(B/(Path(src).stem+'.o')) for src in sources],'-o',str(B/'billing_split')])
split=command([str(B/'billing_split')],input=(R/'Task09/input_valid.txt').read_text());assert split==(R/'Task09/valid_output.txt').read_text()
(R/'Task08/separate_build_output.txt').write_text(split)
e=compile('smart',['Task12/main.c','Task12/smart.c']);out=run('Task12','output.txt',e,['Task12/tariff.cfg'],(R/'Task12/intervals.txt').read_text());assert 'total=Rs 1130.00' in out
cfg=(R/'Task12/tariff.cfg').read_text();(R/'Task12/revised_tariff.cfg').write_text(cfg.replace('rate2_paise=225','rate2_paise=230'))
out=run('Task12','revised_output.txt',e,['Task12/revised_tariff.cfg'],(R/'Task12/intervals.txt').read_text());assert 'total=Rs 1136.00' in out
for name,bad in [('duplicate_key',cfg+'rate2_paise=225\n'),('bad_percent',cfg.replace('peak_percent=20','peak_percent=101')),('negative_rate',cfg.replace('rate2_paise=225','rate2_paise=-1'))]:
 (R/'Task12'/f'{name}.cfg').write_text(bad);run('Task12',f'{name}_output.txt',e,[f'Task12/{name}.cfg'],(R/'Task12/intervals.txt').read_text(),2)
for name,data in [('gap','1 0 0 1000\n1 2 1000 2000\n'),('decrease','1 0 2000 1000\n'),('too_large','1 0 0 1000001\n'),('duplicate','1 0 0 1000\n1 0 1000 2000\n')]:
 (R/'Task12'/f'{name}.txt').write_text(data);run('Task12',f'{name}_output.txt',e,['Task12/tariff.cfg'],data,2)
e=compile('smart_tests',['Task12/tests.c','Task12/smart.c']);run('Task12','tests_output.txt',e,['Task12/tariff.cfg'])
e=compile('smart_tests_sanitized',['Task12/tests.c','Task12/smart.c'],['-O1','-fsanitize=address,undefined','-fno-omit-frame-pointer']);run('Task12','sanitizer_output.txt',e,['Task12/tariff.cfg'])
data='9'*300+'\n0003 D 0 150 N\n'
result=command([str(B/'billing')],expected=2,input=data)
assert 'line too long' in result and '0003 units=150' in result
(R/'Task09/overlong_output.txt').write_text(result)
(R/'compiler.txt').write_text(command(['gcc','--version']))
for row in commands:
 row['command']=[arg.replace(str(B),'build').replace(str(R)+'/', '') for arg in row['command']]
(R/'execution_commands.json').write_text(json.dumps(commands,indent=2))
print('All compiled programs and expected-error checks passed.',flush=True)
