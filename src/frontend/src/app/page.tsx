'use client';
import { useEffect, useState } from 'react';
import Editor from 'react-simple-code-editor';
import Prism from 'prismjs';
import 'prismjs/components/prism-sql';
import 'prismjs/themes/prism-tomorrow.css';
import Table from './components/Table';

export default function Home() {
  const [data, setData] = useState([]);
  const [code, setCode] = useState(
    `CREATE TABLE Customer FROM FILE "C:\\data.csv" USING Hash("DNI");\n`
  );

  useEffect(() => {
    fetch('http://0.0.0.0:18080/query')
      .then((res) => res.json())
      .then((data) => setData(data));
  }, []);

  return (
    <main className="flex flex-col max-w-7xl gap-3 p-20 mx-auto">
      <section className="flex flex-row justify-between w-full rounded-md p-3 outline outline-1 outline-[#D4D4D4]">
        <div className="flex gap-3">
          <p>MyDB.db</p>
          <p className='font-code text-sm content-center'>~/Customer</p>
        </div>
        <div className="flex">
          <p>v1.0.0</p>
        </div>
      </section>

      <section className="flex flex-row justify-between w-full h-32 rounded-md p-3 outline outline-1 outline-[#D4D4D4]">
        <Editor
          value={code}
          onValueChange={code => setCode(code)}
          highlight={code => Prism.highlight(code, Prism.languages.sql, 'sql')}
          padding={4}
          style={{
            fontFamily: '"Fira code", "Fira Mono", monospace',
            fontSize: 12,
            width: '100%',
          }}
        />
      </section>

      <section className="flex flex-row justify-between w-full rounded-md p-3 outline outline-1 outline-[#D4D4D4]">

      </section>

      <section className="flex flex-row justify-between w-full rounded-md p-3 outline outline-1 outline-[#D4D4D4]">
        <Table data={data} />
      </section>
    </main>
  );
}
